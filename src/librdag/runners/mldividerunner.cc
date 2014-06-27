/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for licence.
 *
 */

#include "dispatch.hh"
#include "runners.hh"
#include "expression.hh"
#include "iss.hh"
#include "terminal.hh"
#include "uncopyable.hh"
#include "lapack.hh"
#include "equals.hh"

#include <stdio.h>
#include <complex>
#include <sstream>

using namespace std;

/*
 * Unit contains code for MLDIVIDE node runners
 */

namespace librdag
{

/**
 * Local implementation details.
 */
namespace detail
{

/**
 * Template to create a "real" number in a given domain.
 */
template<typename T> T ndm(real8 val) {}

template<> real8 ndm(real8 val)
{
  return val;
}

template<> complex16 ndm(real8 val)
{
  return complex16(val, 0.e0);
}

/**
 * Casts a value to a char
 * @param the value to cast
 * @return the char representation of the value
 */
template<typename T> char asChar(T value)
{
  return static_cast<char>(value);
}

// Auxiliary structs for holding intermediary guiding data


// NOTE: The following enums are used in some cases to pass flags to lapack.
// Ideally lapack:: members holding `char *` would be used, but these are
// declared extern which conflicts with the constexpr needed in the following.
// At some point, for completeness, this could be fixed.

/**
 * UPLO flags upper or lower triangular matrix.
 */
enum class UPLO: char
{
  /**
   * Flags neither upper or lower.
   */
  NEITHER = 'N',
  /**
   * Flags an upper triangular matrix.
   */
  UPPER = 'U',
  /**
   * Flags a lower triangular matrix.
   */
  LOWER = 'L'
};

/**
 * UNITDIAG flags if a triangular matrix has a unit diagonal.
 */
enum class UNITDIAG: char
{
  /**
   * Flags a unit diagonal.
   */
  UNIT    = 'U',
  /**
   * Flags a non-unit diagonal.
   */
  NONUNIT = 'N'
};

/**
 * PERMUTATION flags the possible permutation type identifiable in a triangular matrix.
 */
enum class PERMUTATION: char
{
  /**
   * Flags as standard, no permutation present.
   */
  STANDARD = 'S',
  /**
   * Flags as a row permutation.
   */
  ROW      = 'R',
  /**
   * Flags as a column permutation.
   */
  COLUMN   = 'C',
  /**
   * Flags as not triangular.
   */
  NOTTRIANGULAR = 'N'
};

/**
 * This struct holds the state from deciding if a matrix is in some way triangular.
 */
struct TriangularStruct
{
  UPLO flagUPLO = UPLO::NEITHER;
  UNITDIAG flagDiag = UNITDIAG::NONUNIT;
  PERMUTATION flagPerm = PERMUTATION::STANDARD;
  unique_ptr<size_t[]> perm = nullptr;
 /**
  * Flags are based on LAPACK chars
  * @param flagUPLO is it upper or lower
  * @param flagDiag is it diagonal
  * @param flagPerm is it permuted
  * @param perm the permutation applied, can be null if \a flagPerm is \a PERMUTATION::STANDARD
  */
};

// These are helper routines

/**
 * Checks if a matrix is symmetric.
 * @param data the data from the matrix, column major
 * @param rows the number of rows
 * @param cols the number of columns
 * @return true if symmetric false else.
 */
template<typename T>
bool isSymmetric(T * data, size_t rows, size_t cols)
{
  size_t ir;
  for (size_t i = 0; i < cols; i++)
  {
    ir = i * rows;
    for (size_t j = 0; j < rows; j++)
    {
      if (data[ir + j] != data[j * rows + i])
      {
        return false;
      }
    }
  }
  return true;
}


/**
 * Checks if a matrix is lower triangular
 */
template<typename T>
void checkIfLowerTriangular(T * data, size_t rows, size_t cols, UPLO* tri, UNITDIAG* diag)
{
  // check lower
  size_t ir;
  *tri = UPLO::LOWER;
  *diag = UNITDIAG::UNIT;
  bool isUnit = true;
  if (!SingleValueFuzzyEquals(data[0],ndm<T>(1.e0),std::numeric_limits<real8>::epsilon(),std::numeric_limits<real8>::epsilon()))
  {
    *diag = UNITDIAG::NONUNIT;
  }
  for (size_t  i = 1; i < cols; i++)
  {
    ir = i * rows;
    // check if diag == 1
    if (isUnit)
    {
      if(!SingleValueFuzzyEquals(data[ir +i],1.e0,std::numeric_limits<real8>::epsilon(),std::numeric_limits<real8>::epsilon()))
      {
        cout << "13. checking lower:: not unit diag at element [" << i << "," << i << "]" << "found value = "<< data[ir + i] << std::endl;
        *diag = UNITDIAG::NONUNIT;
        isUnit = false;
      }
    }
    for (size_t j = 0; j < i; j++)  // check if upper triangle is empty
    {
      if (!SingleValueFuzzyEquals(data[ir + j],0.e0,std::numeric_limits<real8>::epsilon(),std::numeric_limits<real8>::epsilon()))
      {
        cout << "14. checking lower:: returning as nonzero in upper triangle at [" << j << "," << i << "]" << "found value = "<< data[ir + j] << std::endl;
        SingleValueFuzzyEquals(data[ir +j],0.e0,std::numeric_limits<real8>::epsilon(),std::numeric_limits<real8>::epsilon());
        *tri = UPLO::NEITHER;
        return;
      }
    }
  }
}

/**
 * Checks if a square matrix contains an upper triangular matrix or row permutation of
 */
template<typename T>
void checkIfUpperTriangularPermuteRows(T * data, size_t rows, size_t cols,  TriangularStruct * ret)
{
  unique_ptr<size_t[]> rowStarts (new size_t[rows]);
  std::fill(rowStarts.get(), rowStarts.get() + rows, -1);// -1 is our flag

  std::unique_ptr<bool[]> rowTagPtr (new bool[rows]);
  bool * rowTag = rowTagPtr.get();
  std::fill(rowTag,rowTag+rows,false);

  ret->flagDiag = UNITDIAG::UNIT;
  bool upperTriangleIfValidIspermuted = false;
  bool validPermutation = true;
  bool isUDswitch = true; // is unit diag

  // Check if its lower all 0
  // walk rows to look for when number appears in [0...0, Number....]
  for (size_t i = 0; i < cols; i++)
  {
    for (size_t j = 0; j < rows; j++)
    {
      if (
        rowStarts.get()[j] == -1 // we've not seen this row before
        && // AND
        // it's not got a zero at location j.
        !SingleValueFuzzyEquals(data[i * rows + j],0.e0,std::numeric_limits<real8>::epsilon(),std::numeric_limits<real8>::epsilon())
      )
      {
        if (isUDswitch && !SingleValueFuzzyEquals(data[i * rows + j],1.e0,std::numeric_limits<real8>::epsilon(),std::numeric_limits<real8>::epsilon()))
        {
          cout << "16. checking upper:: not unit diag at element [" << i << "," << j << "]" << std::endl;
          ret->flagDiag = UNITDIAG::NONUNIT; // not unit diagonal
          isUDswitch = false;
        }
        rowStarts.get()[j] = i;
      }
    }
  }
  // Check to see if the matrix is a permutation of an upper triangle
  // Do this by checking that the claimed start of rows cover all columns
  // first set flags, then check flags, complete set needed to assess whether the perm is present
  for (size_t i = 0; i < rows; i++)
  {
    if (!rowTag[rowStarts.get()[i]])
    {
      rowTag[rowStarts.get()[i]] = true;
    }
    if (!upperTriangleIfValidIspermuted && rowStarts.get()[i] != i)
    {
      upperTriangleIfValidIspermuted = true;
    }
  }
  for (size_t i = 0; i < rows; i++)
  {
    if (!rowTag[i])
    {
      validPermutation = false;
      break;
    }
  }
  if (!validPermutation)
  {
    cout << "17. checking upper:: not upper triangular, permuted or otherwise" << std::endl;
    ret->flagPerm = PERMUTATION::NOTTRIANGULAR;
    ret->flagDiag = UNITDIAG::NONUNIT;
  }
  else
  {
    if(upperTriangleIfValidIspermuted)
    {
      // permutation is valid, update struct. foo(rowStarts) = 1: length(rowStarts) gives direct perm
      cout << "18. checking upper:: Row permutation spotted" << std::endl;
      ret->flagPerm = PERMUTATION::ROW;
      ret->perm = std::move(rowStarts);
    }
    else
    {
      cout << "19. checking upper:: standard upper triangle" << std::endl;
      ret->flagPerm = PERMUTATION::STANDARD;
    }
  }
  return;
}


template<typename T>
std::unique_ptr<TriangularStruct> isTriangular(T * data, size_t rows, size_t cols)
{
  std::unique_ptr<TriangularStruct> tptr (new TriangularStruct());
  TriangularStruct * ref = tptr.get();

  // See if it's lower triangular
  checkIfLowerTriangular(data, rows, cols, &(ref->flagUPLO), &(ref->flagDiag));
  if (ref->flagUPLO != UPLO::NEITHER)
  {
    return tptr;
  }

  // See if it's Upper or permuted upper
  checkIfUpperTriangularPermuteRows(data, rows, cols, ref);
  if (ref->flagPerm == PERMUTATION::ROW || ref->flagPerm == PERMUTATION::STANDARD )
  {
    ref->flagUPLO = UPLO::UPPER;
    return tptr;
  }

  // DEFAULT, not triangular in any way
  return tptr;

}


} // end namespace detail

// This is a rough translation of OG mldivide from OG-Maths_Legacy ~2012,
//  which in turn is based on my libllsq from ~2011
template<typename T>
void*
mldivide_dense_runner(RegContainer& reg0, shared_ptr<const OGMatrix<T>> arg0, shared_ptr<const OGMatrix<T>> arg1)
{

  // data from array 1 (A)
  T * ptrdata1 = arg0->getData();
  std::size_t rows1 = arg0->getRows();
  std::size_t cols1 = arg0->getCols();
  int4 int4rows1 = rows1;
  int4 int4cols1 = cols1;
  std::size_t len1 = rows1 * cols1;
  std::unique_ptr<T[]> data1Ptr (new T[len1]);
  T * data1 = data1Ptr.get();
  std::copy(ptrdata1,ptrdata1+len1,data1);

  // data from array 2 (B)
  T * ptrdata2 = arg1->getData();
  std::size_t rows2 = arg1->getRows();
  std::size_t cols2 = arg1->getCols();
  int4 int4rows2 = rows2;
  int4 int4cols2 = cols2;
  std::size_t len2 = rows2 * cols2;
  std::unique_ptr<T[]> data2Ptr (new T[len2]);
  T * data2 = data2Ptr.get();
  std::copy(ptrdata2,ptrdata2+len2,data2);

  // useful vars
  real8 rcond = 0; // estimate of reciprocal condition number
  real8 anorm = 0; // the 1 norm of a matrix
  bool singular = false; // is the matrix identified as singular
  bool attemptQR = true; // should QR decomposition be attempted for singular/over determined systems prior to using SVD?

  //auxiallary vars

  // for pointer switching if a perm is needed but system is bad and needs resetting
  unique_ptr<T[]> triPtr1Ptr = nullptr;
  unique_ptr<T[]> triPtr2Ptr = nullptr;
  T * triPtr1 = nullptr;
  T * triPtr2 = nullptr;

  // pointer switching for case when xgels creates a bigger system than for which there's space
  unique_ptr<T[]> bdata2Ptr = nullptr;

  // the permutation vector, if needed
//   std::size_t * permuteV = nullptr;

  // flow control
  // set if matrix is triangular and singular so that other standard decompositions are skipped and a least squares result is attempted immediately
  bool jmp = false;

  // return item
  OGNumeric::Ptr ret;

  // Approx alg: This is based loosely on my llsq library, see internal OG C code.
  // 1) check if system commutes
  // 2) check if square ? goto 3) : goto 6)
  // 3) check if triangular (or perm of), if so compute rcond, if rcond ok solve else goto 6), if solve fails goto 6)
  // 4) check if symmetric, if so, try cholesky, if decomp fails it's not s.p.d. If decomp succeeds, try solve, if fails, compute rcond and goto 6)
  // 5) Try LUP. If decomp succeeds, try solve, if fails, compute rcond and goto 6)
  // 6) rcond+1!=1 ? try QR based llsq : goto 8)
  // 7) if QR decomp succeeds return, else goto 8)
  // 8) run svd based llsq
  // 9) if svd llsq fails warn always return.

  // First...

  // LAPACK info
  int4 info = 0;

  // check if the system is sane
  if(rows1!=rows2)
  {
    stringstream msg;
    msg << "System does not commute. Rows in arg0: " << rows1 << ". Rows in arg1 " << rows2 << std::endl;
    throw rdag_error(msg.str());
  }

  bool debug_ = true;

  // Do the alg above:
  // is it square?
  if (rows1 == cols1)
  {
    if (debug_)
    {
      cout << "10. Matrix is square"  << std::endl;
    }

    // is the array1 triangular or permuted triangular
    unique_ptr<detail::TriangularStruct> tptr = detail::isTriangular(data1, rows1, cols1);
    detail::UPLO UPLO = tptr->flagUPLO;
    detail::UNITDIAG DIAG = tptr->flagDiag;
    if (UPLO != detail::UPLO::NEITHER)  // i.e. this is some breed of triangular
    {
      if (debug_)
      {
        cout << "20. Matrix is " << (UPLO == detail::UPLO::UPPER ? "upper" : "lower") << " triangular, " << (DIAG == detail::UNITDIAG::NONUNIT ? "non-unit" : "unit") << " diagonal." << std::endl;
      }
      detail::PERMUTATION DATA_PERMUTATION = tptr->flagPerm;
      if (DATA_PERMUTATION != detail::PERMUTATION::STANDARD)   // we have a permutation of the data, rewrite
      {
        if (debug_)
        {
          cout << "25. Matrix is " << (DATA_PERMUTATION == detail::PERMUTATION::ROW ? "row" : "column") << " permuted triangular." << std::endl;
        }
//         permuteV = tptr->perm.get(); // the permutation
        if (DATA_PERMUTATION == detail::PERMUTATION::ROW)
        {
          triPtr1Ptr = unique_ptr<T[]>(new T[len1]());
          triPtr1Ptr.swap(data1Ptr); // triPtr1Ptr now points to original data
          // assign pointers for read/write ops
          triPtr1  = triPtr1Ptr.get();
          data1 = data1Ptr.get();
          std::fill(data1,data1+len1,0e0);
          for (size_t i = 0; i < cols1; i++)
          {
            for (size_t j = 0; j < rows1; j++)
            {
              data1[i * rows1 + tptr->perm.get()[j]] = triPtr1[i * rows1 + j];
            }
          }
          triPtr2Ptr = unique_ptr<T[]>(new T[len2]());
          triPtr2Ptr.swap(data2Ptr); // triPtr2Ptr now points to original data
          // assign pointers for read/write ops
          triPtr2  = triPtr2Ptr.get();
          data2 = data2Ptr.get();
          std::fill(data2,data2+len2,0e0);
          for (size_t i = 0; i < cols2; i++)
          {
            for (size_t j = 0; j < rows2; j++)
            {
              data2[i * rows2 + tptr->perm.get()[j]] = triPtr2[i * rows2 + j];
            }
          }
        }
      } // end permutation branch


      cout << "28. Hitting lapack tri routines with UPLO = " << (UPLO == detail::UPLO::UPPER ? "upper" : "lower") << " DIAG = " << (DIAG == detail::UNITDIAG::NONUNIT ? "non-unit" : "unit") << std::endl;

      // compute reciprocal condition number, if it's bad say so and least squares solve
//       _lapack.dtrcon('1', UPLO, DIAG, rows1, data1, rows1, rcond, work, iwork, info);
      auto lapack_uplo = asChar(UPLO);
      auto lapack_diag = asChar(DIAG);
      lapack::xtrcon(lapack::ONE, &lapack_uplo, &lapack_diag, &int4rows1, data1, &int4rows1, &rcond, &info);

      if (rcond + 1.e0 != 1.e0)// rcond ~< D1mach(4)
      {
//         _lapack.dtrtrs(UPLO, 'N', DIAG, rows1, cols2, data1, rows1, data2, rows2, info);
        info = 0;
        try
        {
          lapack::xtrtrs(&lapack_uplo, lapack::N, &lapack_diag, &int4rows1, &int4cols2, data1, &int4rows1, data2, &int4rows2, &info);
        }
        catch (rdag_error& e)
        {
          if(info<0) // caught a xerbla error, rethrow
          {
            throw;
          }
          if (debug_)
          {
            cout<<("40. Triangular solve fail. Mark as singular.");
          }
          singular = true;
        }

        if (debug_)
        {
          cout << "30. Triangular solve success, returning";
        }
        ret = makeConcreteDenseMatrix(data2Ptr.release(), rows2, cols2, OWNER);
        reg0.push_back(ret);
        return nullptr;
      }
      else
      {
        if (debug_)
        {
          cout << "43. Triangular condition was computed as too bad to attempt solve." << std::endl;
        }
        singular = true;
      }

      // reset permutation, just a pointer switch, not doing so might influence results from
      // iterative llsq solvers
      if (DATA_PERMUTATION != detail::PERMUTATION::STANDARD)
      {
        if(debug_)
        {
          cout << "45. Resetting permutation." << std::endl;
        }

        // swap back pointers
        data1Ptr.swap(triPtr1Ptr);
        data2Ptr.swap(triPtr2Ptr);
        data1 = data1Ptr.get();
        data2 = data2Ptr.get();
      }
      jmp = true; // nmatrix is singular, jump to least squares logic

    } // end "this matrix is triangular"

    // if it's triangular and it failed, then it's singular and a condition number is already computed and this can be jumped!
    if (!jmp)
    {
      if (debug_)
      {
        cout << "50. Not triangular" << std::endl;
      }
      // see if it's Hermitian (symmetric in the real case)
      // stu - this needs looking at, complex case could be symmetric or Hermitian, in which case
      // different specialisations are available, template in a thing to deal with this...
      if (detail::isSymmetric(data1, rows1, cols1))
      {
        if (debug_)
        {
          cout << "60. Is Hermitian" << std::endl;
        }
        // cholesky decompose, shove in lower triangle
//         _lapack.dpotrf('L', rows1, data1, rows1, info);
        info = 0;
        try
        {
          lapack::xpotrf(lapack::L, &int4rows1, data1, &int4rows1, &info);
        }
        catch(rdag_error& e)
        {
          if(info < 0)
          {
            throw;
          }
        }

        if(info == 0)
        {
          //         if (info[0] == 0) { // Cholesky factorisation was ok, matrix is s.p.d. Factorisation is in the lower triangle, back solve based on this
          if (debug_)
          {
            cout << "70. Cholesky success.  Computing condition based on cholesky factorisation" << std::endl;
          }
          //           anorm = _lapack.dlansy('1', 'L', rows1, array1.getData(), rows1, work);
          anorm = lapack::xlansy(lapack::ONE, lapack::L, &int4rows1, data1, &int4rows1);
          // Cholesky condition estimate
          lapack::xpocon(lapack::L, &int4rows1, data1, &int4rows1, &anorm, &rcond, &info);
          if (debug_)
          {
            cout << "80. Cholesky condition estimate. " << rcond << std::endl;
          }
          if (1.e0 + rcond != 1.e0)
          {
            if (debug_)
            {
              cout << "90. Cholesky condition acceptable. Backsolve and return." << std::endl;
            }
            lapack::xpotrs(lapack::L, &int4rows1, &int4cols2, data1, &int4rows1, data2, &int4rows2, &info);
            // info[0] will be zero. Any -ve info[0] will be handled by XERBLA
            ret = makeConcreteDenseMatrix(data2Ptr.release(), rows2, cols2, OWNER);
            reg0.push_back(ret);
            return nullptr;
          }
          else
          {
            if (debug_)
            {
              cout << "100. Cholesky condition bad. Mark as singular." << std::endl;
            }
            singular = true;
          }
        }
        // stu - this branch from "if (info[0] == 0)" following xpotrf call is now dead
        // lapack will throw if there's a problem, it's an input error if {xerbla} or {not spd}
//         } else { // factorisation failed
//           if (debug_) {
//            cout << "110. Cholesky factorisation failed. Matrix is not s.p.d." << std::endl;
//           }
//         } // end factorisation info==0
      } // end symmetric branch

      if (!singular)
      {
        if (debug_)
        {
          cout << "120. Non-symmetric OR not s.p.d." << std::endl;
        }
        //  we're here as matrix isn't s.p.d. as Cholesky failed. Get new copy of matrix
//         System.arraycopy(array1.getData(), 0, data1, 0, array1.getData().length);
        std::copy(arg0->getData(),arg0->getData()+len1,data1);
        // try solving with generalised LUP solver
//         int[] ipiv = new int[rows1];
        unique_ptr<int[]> ipivPtr (new int4[rows1]);
        // decompose
        if (debug_)
        {
          cout << "130. LUP attempted" << std::endl;
        }
//         _lapack.dgetrf(rows1, cols1, data1, rows1, ipiv, info);
        try
        {
          lapack::xgetrf(&int4rows1, &int4cols1, data1, &int4rows1, ipivPtr.get(), &info);
        }
        catch (rdag_error& e)
        {
          if(info<0) // caught a xerbla error, rethrow
          {
            throw;
          }
        }

        if (info == 0)
        {
          if (debug_)
          {
            cout << "140. LUP success. Computing condition based on LUP factorisation." << std::endl;
          }
//           anorm = _lapack.dlange('1', rows1, cols1, array1.getData(), rows1, work);
          anorm = lapack::xlange(lapack::ONE, &int4rows1, &int4cols1, arg0->getData(), &int4rows1);

//           _lapack.dgecon('1', rows1, data1, rows1, anorm, rcond, work, iwork, info);
          lapack::xgecon(lapack::ONE, &int4rows1, data1, &int4rows1, &anorm, &rcond, &info);

          // if condition estimate isn't too bad then back solve
          if (1.e0 + rcond != 1.e0)
          {
            // back solve dgetrs()
//             _lapack.dgetrs('N', cols1, cols2, data1, cols1, ipiv, data2, cols1, info);
            lapack::xgetrs(lapack::N, &int4cols1, &int4cols2, data1, &int4cols1, ipivPtr.get(), data2, &int4cols1, &info);
            if (debug_)
            {
              cout << "150. LUP returning" << std::endl;
            }
//             return new OGMatrix(data2, rows2, cols2);
            ret = makeConcreteDenseMatrix(data2Ptr.release(), rows2, cols2, OWNER);
            reg0.push_back(ret);
            return nullptr;
          }
          else     // condition bad, mark as singular
          {
            if (debug_)
            {
              cout << "160. LUP failed, condition too high" << std::endl;
            }
            singular = true;
          }
        }
        else
        {
          if (debug_)
          {
            cout << "170. LUP factorisation failed. Mark as singular." << std::endl;
          }
          singular = true;
        }
      }
    }
  } // end if square branch

  // branch on rcond if computed, if cond is sky high then rcond is near zero, we use 1 here so cutoff is near 1e-16
  if (singular)
  {
    cout << "Square matrix is singular to machine precision. RCOND estimate = " << rcond << std::endl;
    if (debug_)
    {
      cout << "190. Square matrix is singular." << std::endl;
    }
    if ((rcond + 1.e0) == 1.e0)
    {
      if (debug_)
      {
        cout << "200. Condition of square matrix is too bad for QR least squares." << std::endl;
      }
      attemptQR = false;
    }
    // take a copy of the original data as it will have been destroyed above
//       System.arraycopy(array1.getData(), 0, data1, 0, array1.getData().length);
    std::copy(arg0->getData(),arg0->getData()+len1,data1Ptr.get());
  }

  // needed for QR and SVD
  int4 ldb = std::max(rows1, cols1);
  if (attemptQR)
  {
    if (debug_)
    {
      cout << "210. Attempting QR solve." << std::endl;
    }

    // malloc some space for the solution as it will be rows1 * cols2 in size and data2 is rows2*cols2 which may not be big enough
    if (rows1 < cols1)
    {
      bdata2Ptr = unique_ptr<T[]>(new T[ldb * cols2]);
      T * b = bdata2Ptr.get();
      //copy in data strips
      for (size_t i = 0; i < cols2; i++)
      {
        std::copy(data2 + (i * rows2), data2 + ((i + 1)* rows2), b + i * ldb);
      }
      // switch pointers so data2 now points at a correct sized alloc
      data2Ptr.swap(bdata2Ptr);
      // reassign underlying data2 pointer
      data2 = data2Ptr.get();
    }

    // copy in the data
//       _lapack.dgels('N', rows1, cols1, cols2, data1, rows1, data2, ldb, dummywork, lwork, info);
//       lwork = (int) dummywork[0];
//       work = new double[lwork];
//       _lapack.dgels('N', rows1, cols1, cols2, data1, rows1, data2, ldb, work, lwork, info);
    try
    {
      lapack::xgels(lapack::N, &int4rows1, &int4cols1, &int4cols2, data1, &int4rows1, data2, &ldb, &info);
    }
    catch(rdag_error& e)
    {
      if (info < 0)
      {
        throw;
      }
    }

    if (info > 0)
    {
      if (debug_)
      {
        cout <<  "220. QR solve failed" << std::endl;
      }
      cout <<  " WARN: Matrix of coefficients does not have full rank. Rank is " << info << "." << std::endl;
      // switch back to original data if we created a "b"
      if (rows1 < cols1)
      {
        data2Ptr.swap(bdata2Ptr);
        // reassign underlying data2 pointer
        data2 = data2Ptr.get();
      }

      // take a copy of the original data as it will have been destroyed above
//         System.arraycopy(array1.getData(), 0, data1, 0, array1.getData().length);
//         System.arraycopy(array2.getData(), 0, data2, 0, array2.getData().length);
      std::copy(arg0->getData(),arg0->getData()+len1,data1);
      std::copy(arg1->getData(),arg1->getData()+len2,data2);
    }
    else
    {
      if (debug_)
      {
        cout << "230. QR solve success, returning" << std::endl;
      }
      // 1:cols1 from each column of data2 needs to be returned
      T * ref = data2;
      data2 = new T[cols1 * cols2];
      for (size_t i = 0; i < cols2; i++)
      {
//           System.arraycopy(ref, i * ldb, data2, i * cols1, cols1);
        std::copy(ref + (i * ldb), ref + ((i * ldb)+cols1), data2 + (i * cols1));
      }
//         return new OGMatrix(data2, cols1, cols2);
      ret = makeConcreteDenseMatrix(data2, cols1, cols2, OWNER);
      reg0.push_back(ret);
      return nullptr;
    }
  }

  if (debug_)
  {
    cout <<  "240. Attempting SVD" << std::endl;
  }

  // so we attempt a general least squares solve
  unique_ptr<real8[]> sPtr (new real8[std::min(rows1, cols1)]());
  real8 moorePenroseRcond = -1; // this is the definition of singular in the Moore-Penrose sense, if set to -1 machine prec is used
//     int[] rank = new int[1];
//
//     // internal calls in svd to ilaenv() to get parameters for svd call seems broken in netlib jars, will need to patch the byte code or override
//     lwork = -1;
//     _lapack.dgelsd(rows1, cols1, cols2, data1, Math.max(1, rows1), data2, Math.max(1, Math.max(rows1, cols1)), s, moorePenroseRcond, rank, dummywork, lwork, dummyiwork, info);
//     lwork = (int) dummywork[0];
//     work = new double[lwork];
//     iwork = new int[dummyiwork[0]];
//
//     // make the call to the least squares solver
//     _lapack.dgelsd(rows1, cols1, cols2, data1, Math.max(1, rows1), data2, Math.max(1, Math.max(rows1, cols1)), s, moorePenroseRcond, rank, work, lwork, iwork, info);

  int4 rank=0;
  try
  {
    lapack::xgelsd(&int4rows1, &int4cols1, &int4cols2, data1, &int4rows1, data2, &ldb, sPtr.get(), &moorePenroseRcond, &rank, &info);
  }
  catch (rdag_error& e)
  {
    if(info<0)
    {
      throw;
    }
  }

  // handle fail on info
  if (info != 0)
  {
    if (debug_)
    {
      cout << "250. SVD failed" << std::endl;
    }
    cout << "SVD Failed to converege. " << info << " out of " << std::max(rows1, cols1) << " off diagonals failed to converge to zero. RCOND = " << rcond << std::endl;
  }
  else
  {
    if (debug_)
    {
      cout << "260. SVD success" << std::endl;
    }
  }
  if (debug_)
  {
    cout << "270. SVD returning" << std::endl;
  }
  //     return new OGMatrix(Arrays.copyOf(data2, cols1 * cols2), cols1, cols2);
  // stu - technically the wrong size data *but* C just sees a pointer
  ret = makeConcreteDenseMatrix(data2Ptr.release(), cols1, cols2, OWNER);
  reg0.push_back(ret);
  return nullptr;
}


// MLDIVIDE runner:
void * MLDIVIDERunner::run(RegContainer& reg0, OGComplexDenseMatrix::Ptr arg0, OGComplexDenseMatrix::Ptr arg1) const
{
  mldivide_dense_runner<complex16>(reg0, arg0, arg1);
  return nullptr;
}


void * MLDIVIDERunner::run(RegContainer& reg0, OGRealDenseMatrix::Ptr arg0, OGRealDenseMatrix::Ptr arg1) const
{
  mldivide_dense_runner<real8>(reg0, arg0, arg1);
  return nullptr;
}

void *
MLDIVIDERunner::run(RegContainer& reg0, OGRealScalar::Ptr arg0, OGRealScalar::Ptr arg1) const
{
  real8 data1 = arg0->getValue();
  real8 data2 = arg1->getValue();
  OGNumeric::Ptr ret = OGRealScalar::create(data1+data2);
  reg0.push_back(ret);
  return nullptr;
}

}

