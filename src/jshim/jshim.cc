#include "com_opengamma_longdog_materialisers_Materialisers.h"
#include "entrypt.h"
#include "jshim.h"
#include "expression.hh"
#include "exprtypeenum.h"
#include "warningmacros.h"
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

namespace convert
{
using namespace librdag;

/*
 * General execption thing
 */
class convertException: public exception
{
    virtual const char* what() const throw()
    {
      return "Exception occurred.";
    }
} convertExcept;


/**
 * helper function to get ints from int getFOO() in java
 */
jint getIntFromVoidJMethod(jmethodID id, jobject obj)
{
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
#ifdef _DEBUG
    printf("Thread attach failed\n");
#endif
    exit(1);
  }
  jint data = 0x7ff00000;
  data = env->CallIntMethod(obj, id);
  return data;
}

/*
 * Template signatures
 */

/*
 * Binds some OGArray data in an OGArray object obj to a type T (T extends double)
 */
template <typename T> T * bindOGArrayData(jobject obj);
/*
 * UnBinds some OGArray data of type "T" (T extends double) from an OGArray object "obj"
 */
template <typename T> void unbindOGArrayData(T * nativeData, jobject obj);
/*
 * Binds primitive array data of type "T" from an OGArray object "obj" accessed by getter method "method"
 */
template <typename T, typename S> T * bindPrimitiveArrayData(jobject obj, jmethodID method);
/*
 * UnBinds primitive array data "nativeData" of type "T" from an OGArray object "obj" accessed by getter method "method"
 */
template <typename T, typename S> void unbindPrimitiveArrayData(T * nativeData, jobject obj, jmethodID method);

/*
 * An OGRealMatrix backed by data pinned from a Java based OGRealMatrix
 */
class JOGRealMatrix: public OGRealMatrix
{
  public:
    JOGRealMatrix(jobject * obj)
    {
      this->_backingObject = obj;
      real16 * _dataptr = bindPrimitiveArrayData<real16, jdoubleArray>(*obj, OGTerminalClazz_getData);
      this->noCopy_ctor(_dataptr,getIntFromVoidJMethod(OGArrayClazz_getRows, *obj),getIntFromVoidJMethod(OGArrayClazz_getCols, *obj));
    };
    ~JOGRealMatrix()
    {
      unbindOGArrayData<real16>(this->getData(), *_backingObject);
    };
    void debug_print()
    {
      printf("\nJava bound OGRealMatrix\n");
      size_t ptr=0;
      printf("\n");
      for(int i = 0 ; i < this->getRows(); i++)
      {
        for(int j = 0 ; j < this->getCols()-1; j++)
        {
          printf("%6.4f, ",this->getData()[ptr++]);
        }
        printf("%6.4f\n",this->getData()[ptr++]);
      }
    }
  private:
    jobject * _backingObject = NULL;
};

/*
 * An OGComplexMatrix backed by data pinned from a Java based OGComplexMatrix
 */
class JOGComplexMatrix: public OGComplexMatrix
{
  public:
    JOGComplexMatrix(jobject * obj)
    {
      this->_backingObject = obj;
      complex16 * _dataptr = bindPrimitiveArrayData<complex16, jdoubleArray>(*obj,OGTerminalClazz_getData);
      this->noCopy_ctor(_dataptr,getIntFromVoidJMethod(OGArrayClazz_getRows, *obj),getIntFromVoidJMethod(OGArrayClazz_getCols, *obj));
    };
    ~JOGComplexMatrix()
    {
      unbindOGArrayData<complex16>(this->getData(), *_backingObject);
    };
    void debug_print()
    {
      printf("\nJava bound OGComplexMatrix\n");
      size_t ptr=0;
      printf("\n");
      for(int i = 0 ; i < this->getRows(); i++)
      {
        for(int j = 0 ; j < this->getCols()-1; j++)
        {
          ptr++;
          printf("%6.4f + %6.4fi, ",this->getData()[ptr].real(),this->getData()[ptr].imag());
        }
        printf("%6.4f + %6.4fi\n",this->getData()[ptr].real(),this->getData()[ptr].imag());
        ptr++;
      }
    }      
  private:
    jobject * _backingObject = NULL;
};

/*
 * An OGRealSparseMatrix backed by data pinned from a Java based OGRealSparseMatrix
 */
class JOGRealSparseMatrix: public OGRealSparseMatrix
{
  public:
    JOGRealSparseMatrix(jobject * obj)
    {
      this->_backingObject = obj;
      real16 * _dataptr = bindPrimitiveArrayData<real16, jdoubleArray>(*obj, OGTerminalClazz_getData);
      int * _colPtr = bindPrimitiveArrayData<int, jintArray>(*obj, OGSparseMatrixClazz_getColPtr);
      int * _rowIdx = bindPrimitiveArrayData<int, jintArray>(*obj, OGSparseMatrixClazz_getRowIdx);
      this->noCopy_ctor(_colPtr, _rowIdx, _dataptr,getIntFromVoidJMethod(OGArrayClazz_getRows, *obj),getIntFromVoidJMethod(OGArrayClazz_getCols, *obj));
    };
    ~JOGRealSparseMatrix()
    {
      unbindPrimitiveArrayData<real16, jdoubleArray>(this->getData(), *_backingObject, OGTerminalClazz_getData);
      unbindPrimitiveArrayData<int, jintArray>(this->getColPtr(), *_backingObject, OGSparseMatrixClazz_getColPtr);
      unbindPrimitiveArrayData<int, jintArray>(this->getRowIdx(), *_backingObject, OGSparseMatrixClazz_getRowIdx);
    };
    void debug_print()
    {
      double nnz = 100.e0 * this->getDatalen() / (this->getRows() * this->getCols());
      printf("\nJava bound OGRealSparseMatrix\n");
      printf("\ndata len = %d\n",this->getDatalen());
      printf("[nnz density = %4.2f. rows = %d, columns = %d]\n", nnz, this->getRows(), this->getCols());
      int * colPtr = this->getColPtr();
      for (int ir = 0; ir < this->getCols(); ir++)
      {
        for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++)
        {
          printf("(%d,%d) = %6.4f\n",this->getRowIdx()[i],ir,this->getData()[i]);
        }
      }
    }
    real16 ** toReal16ArrayOfArrays() override {
      double ** foo = NULL;
      printf("returning null as no impl yet!!!!\n");
      return foo;
    }   
    complex16 ** toComplex16ArrayOfArrays() override {
      complex16 ** foo = NULL;
      printf("returning null as no impl yet!!!!\n");
      return foo;
    }        
  private:
    jobject * _backingObject = NULL;
};

/*
 * An OGComplexSparseMatrix backed by data pinned from a Java based OGComplexSparseMatrix
 */
class JOGComplexSparseMatrix: public OGComplexSparseMatrix
{
  public:
    JOGComplexSparseMatrix(jobject * obj)
    {
      this->_backingObject = obj;
      complex16 * _dataptr = bindPrimitiveArrayData<complex16, jdoubleArray>(*obj, OGTerminalClazz_getData);
      int * _colPtr = bindPrimitiveArrayData<int, jintArray>(*obj, OGSparseMatrixClazz_getColPtr);
      int * _rowIdx = bindPrimitiveArrayData<int, jintArray>(*obj, OGSparseMatrixClazz_getRowIdx);
      this->noCopy_ctor(_colPtr, _rowIdx, _dataptr,getIntFromVoidJMethod(OGArrayClazz_getRows, *obj),getIntFromVoidJMethod(OGArrayClazz_getCols, *obj));
    };
    ~JOGComplexSparseMatrix()
    {
      unbindPrimitiveArrayData<complex16, jdoubleArray>(this->getData(), *_backingObject, OGTerminalClazz_getData);
      unbindPrimitiveArrayData<int, jintArray>(this->getColPtr(), *_backingObject, OGSparseMatrixClazz_getColPtr);
      unbindPrimitiveArrayData<int, jintArray>(this->getRowIdx(), *_backingObject, OGSparseMatrixClazz_getRowIdx);
    };
    void debug_print()
    {
      double nnz = 100.e0 * this->getDatalen()/ (this->getRows() * this->getCols());
      printf("\nJava bound OGComplexSparseMatrix\n");
      printf("datalen:%d\n",this->getDatalen());
      printf("rows:%d\n",this->getRows());
      printf("cols:%d\n",this->getCols());
      printf("[nnz density = %4.2f. rows = %d, columns = %d]\n", nnz, this->getRows(), this->getCols());
      int * colPtr = this->getColPtr();
      for (int ir = 0; ir < this->getCols(); ir++)
      {
        for (int i = colPtr[ir]; i < colPtr[1]; i++)
        {
          printf("(%d,%d) = %6.4f + %6.4fi \n",this->getRowIdx()[i],ir,this->getData()[i].real(),this->getData()[i].imag());
        }
      }
    }
    real16 ** toReal16ArrayOfArrays() override {
      double ** foo = NULL;
      printf("returning null as no impl yet!!!!\n");
      return foo;
    }  
    complex16 ** toComplex16ArrayOfArrays() override {
      complex16 ** foo = NULL;
      printf("returning null as no impl yet!!!!\n");
      return foo;
    }        
  private:
    jobject * _backingObject = NULL;
};


/**
 * binds the data in an OGArray class to a T pointer
 * @param T the class of the underlying data
 * @param S the class representation in the pinning call
 * @param obj the object from which the data shall be extracted
 * @param method the method to "get" the native data reference
 */
template <typename T, typename S> T * bindPrimitiveArrayData(jobject obj, jmethodID method)
{
  if(!obj)
  {
#ifdef _DEBUG
    printf("bindPrimitiveArrayData: null obj\n");
#endif
    exit(1);
  }
  if(!method)
  {
#ifdef _DEBUG
    printf("bindPrimitiveArrayData: null method\n");
#endif
    exit(1);
  }
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
#ifdef _DEBUG
    printf("Thread attach failed\n");
#endif
    exit(1);
  }
  jobject dataobj = NULL;
  dataobj = env->CallObjectMethod(obj, method);
  if(!dataobj)
  {
#ifdef _DEBUG
    printf("CallObjectMethod failed\n");
#endif
    exit(1);
  }
  S * array = reinterpret_cast<S *>(&dataobj);
  T * _dataptr = (T *) env->GetPrimitiveArrayCritical(*array,NULL);
  return _dataptr;
}

/**
 * free (unbind) the data in an OGArray class from a T pointer
 * @param T the class of the nativeData
 * @param S the class representation in the pinning call
 * @param nativeData the native data to unbind
 * @param obj the java object from which the native data is pinned
 * @param method the method that refers to the pinned data
 */
template <typename T, typename S> void unbindPrimitiveArrayData(T * nativeData, jobject obj, jmethodID method)
{
  if(!nativeData)
  {
#ifdef _DEBUG
    printf("unbindPrimitiveArrayData: null nativeData\n");
#endif
    exit(1);
  }
  if(!obj)
  {
#ifdef _DEBUG
    printf("unbindPrimitiveArrayData: null obj\n");
#endif
    exit(1);
  }
  if(!method)
  {
#ifdef _DEBUG
    printf("unbindPrimitiveArrayData: null method\n");
#endif
    exit(1);
  }
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
#ifdef _DEBUG
    printf("Thread attach failed\n");
#endif
    exit(1);
  }
  jobject dataobj = env->CallObjectMethod(obj, method);
  S * array = reinterpret_cast<S *>(&dataobj);
  env->ReleasePrimitiveArrayCritical(*array, (void *)nativeData, 0);
}

/**
 * free (unbind) the data in an OGArray class from a T pointer
 * @param T the class of the nativeData
 * @param nativeData the native data to unbind
 * @param obj the java object from which the native data is pinned
 */
template <typename T> void unbindOGArrayData(T * nativeData, jobject obj)
{
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
#ifdef _DEBUG
    printf("Thread attach failed\n");
#endif
    exit(1);
  }
  jobject dataobj = env->CallObjectMethod(obj, OGTerminalClazz_getData);
  jdoubleArray * array = reinterpret_cast<jdoubleArray *>(&dataobj);
  env->ReleasePrimitiveArrayCritical(*array, (void *)nativeData, 0);
}

/**
 * Spec for an OGExpr node derived from a java object
 */
class JOGExpr
{
  public:
    JOGExpr(jobject * obj);
    virtual ~JOGExpr();
  private:
    jobject * _backingObject = NULL;
  protected:
    std::vector<OGNumeric *> *generateArgs();
};
/**
 * COPY node spec derived from a java COPY node
 */
class JCOPY: public JOGExpr, public COPY
{
  public:
    static JCOPY* fromJObject(jobject *obj);
    JCOPY(jobject * obj);
    ~JCOPY();
    void debug_print();
};


/**
 * PLUS node spec derived from a java PLUS node
 */
class JPLUS: public JOGExpr, public PLUS
{
  public:
    static JPLUS* fromJObject(jobject *obj);
    JPLUS(jobject * obj);
    ~JPLUS();
    void debug_print();
};

/**
 * MINUS node spec derived from a java MINUS node
 */
class JMINUS: public JOGExpr, public MINUS
{
  public:
    static JMINUS* fromJObject(jobject *obj);
    JMINUS(jobject * obj);
    ~JMINUS();
    void debug_print();
};

/**
 * SVD node spec derived from a java SVD node
 */
class JSVD: public JOGExpr, public SVD
{
  public:
    static JSVD* fromJObject(jobject *obj);
    JSVD(jobject * obj);
    ~JSVD();
    void debug_print();
};

/**
 * SELECTRESULT node spec derived from a java SVD node
 */
class JSELECTRESULT: public JOGExpr, public SELECTRESULT
{
  public:
    static JSELECTRESULT* fromJObject(jobject *obj);
    JSELECTRESULT(jobject * obj);
    ~JSELECTRESULT();
    void debug_print();
};

/**
 * Generates expression nodes from a java object
 */
class ExprFactory
{
  public:
    ExprFactory() {};
    librdag::OGNumeric * getExpr(jobject obj)
    {
      printf("In exprfactory\n");
#ifdef _DEBUG
      printf("vm ptr at 0x%llx\n",JVMcache);
#endif
      JNIEnv *env=NULL;
      jint jStatus = 0;
      jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
      if(jStatus)
      {
#ifdef _DEBUG
        printf("Thread attach failed\n");
#endif
        exit(1);
      }
      if(!OGNumericClazz_getType)
      {
#ifdef _DEBUG
        printf("Clazz Type is null, 0x%llx\n",OGNumericClazz_getType);
#endif
        exit(1);
      }
      jobject typeobj = env->CallObjectMethod(obj, OGNumericClazz_getType);
      jlong ID = env->GetLongField(typeobj,OGExprTypeEnumClazz__hashdefined);
#ifdef __MINGW32__
      unsigned int high, low;
      low =   (long long unsigned int)ID & 0x00000000FFFFFFFFULL;
      high = ((long long unsigned int)ID & 0xFFFFFFFF00000000ULL) >> 32;
      printf("Clazz Type is hashdefined as %x%x\n", high, low);
#else
      printf("Clazz Type is hashdefined as %lld\n",(long long int)ID);
#endif
      switch(ID)
      {
      case OGREALMATRIX_ENUM:
      {
        printf("Binding a JOGRealMatrix\n");
        _expr = new JOGRealMatrix(&obj);
        _expr->debug_print();
      }
      break;
      case OGCOMPLEXMATRIX_ENUM:
      {
        printf("Binding a JOGComplexMatrix\n");
        _expr = new JOGComplexMatrix(&obj);
        _expr->debug_print();
      }
      break;
      case OGREALSPARSEMATRIX_ENUM:
      {
        printf("Binding a JOGRealSparseMatrix\n");
        _expr = new JOGRealSparseMatrix(&obj);
        _expr->debug_print();
      }
      break;
      case OGCOMPLEXSPARSEMATRIX_ENUM:
      {
        printf("Binding a JOGComplexSparseMatrix\n");
        _expr = new JOGComplexSparseMatrix(&obj);
        _expr->debug_print();
      }
      break;
      case COPY_ENUM:
      {
        printf("COPY function\n");
        _expr = JCOPY::fromJObject(&obj);
      }
      break;
      case MINUS_ENUM:
      {
        printf("MINUS function\n");
        _expr = JMINUS::fromJObject(&obj);
      }
      break;
      case PLUS_ENUM:
      {
        printf("PLUS function\n");
        _expr = JPLUS::fromJObject(&obj);
      }
      break;
      case SVD_ENUM:
      {
        printf("SVD function\n");
        _expr = JSVD::fromJObject(&obj);
      }
      break;
      case SELECTRESULT_ENUM:
      {
        printf("Select Result function\n");
        _expr = JSELECTRESULT::fromJObject(&obj);
      }
      break;
      default:
      {
        throw convertExcept;
      }
      break;
      }

      if(!_expr)
      {
        printf("_expr is NULL so hasn't been set by the factory, exiting\n");
        exit(1);
      }

      printf("Returning from exprfactory\n");
      return _expr;
    };
  private:
    librdag::OGNumeric * _expr = NULL;
};


/**
 * OGExpr implementation details
 * Create an expression node from a java expression node
 */
JOGExpr::JOGExpr(jobject * obj)
{
  if(!obj)
  {
#ifdef _DEBUG
    printf("JOGExpr: null obj\n");
#endif
    exit(1);
  }
  this->_backingObject = obj;
}

std::vector<OGNumeric *> *
JOGExpr::generateArgs()
{
  jobject *obj = this->_backingObject;
  // get object array
  jmethodID method = OGExprClazz_getExprs;
  if(!method)
  {
#ifdef _DEBUG
    printf("JOGExpr: null method\n");
#endif
    exit(1);
  }
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
#ifdef _DEBUG
    printf("Thread attach failed\n");
#endif
    exit(1);
  }
  jobject dataobj = NULL;
  dataobj = env->CallObjectMethod(*obj, method);
  if(!dataobj)
  {
#ifdef _DEBUG
    printf("CallObjectMethod failed\n");
#endif
    exit(1);
  }
  jobjectArray * args = reinterpret_cast<jobjectArray *>(&dataobj);
  jsize len = env->GetArrayLength((jarray)*args);
#ifdef _DEBUG
  printf("JOGExpr arg size is %d\n",len);
#endif
  std::vector<OGNumeric *> * local_args = new std::vector<librdag::OGNumeric *>;
  ExprFactory * factory = new ExprFactory();
  for(int i=0; i<len; i++)
  {

    jobject tmp = (jobject) env->GetObjectArrayElement(*args, i);
    local_args->push_back(factory->getExpr(tmp));
  }
  return local_args;
}

JOGExpr:: ~JOGExpr() {}


/**
 * JCOPY implementation details
 * COPY node derived from a java COPY node
 */
JCOPY::JCOPY(jobject * obj): JOGExpr(obj) { }
void JCOPY::debug_print()
{
  printf("JCOPY class\n");
}

JCOPY*
JCOPY::fromJObject(jobject *obj)
{
  JCOPY *_expr = new JCOPY(obj);
  _expr->setArgs(_expr->generateArgs());
  return _expr;
}

JCOPY::~JCOPY() {}

/**
 * JPLUS implementation details
 * PLUS node derived from a java PLUS node
 */
JPLUS::JPLUS(jobject * obj): JOGExpr(obj) { }

JPLUS*
JPLUS::fromJObject(jobject *obj)
{
  JPLUS *_expr = new JPLUS(obj);
  _expr->setArgs(_expr->generateArgs());
  return _expr;
}

void JPLUS::debug_print()
{
  printf("JPLUS class\n");
}
JPLUS::~JPLUS() {}

/**
 * JMINUS implementation details
 * MINUS node derived from a java MINUS node
 */
JMINUS::JMINUS(jobject * obj): JOGExpr(obj) { }

JMINUS*
JMINUS::fromJObject(jobject *obj)
{
  JMINUS *_expr = new JMINUS(obj);
  _expr->setArgs(_expr->generateArgs());
  return _expr;
}

void JMINUS::debug_print()
{
  printf("JMINUS class\n");
}
JMINUS::~JMINUS() {}

/**
 * JSVD implementation details
 * SVD node derived from a java SVD node
 */
JSVD::JSVD(jobject * obj): JOGExpr(obj) { }

JSVD*
JSVD::fromJObject(jobject *obj)
{
  JSVD *_expr = new JSVD(obj);
  _expr->setArgs(_expr->generateArgs());
  return _expr;
}

void JSVD::debug_print()
{
  printf("JSVD class\n");
}
JSVD::~JSVD() {}

/**
 * JSELECTRESULT implementation details
 * SELECTRESULT node derived from a java SVD node
 */
JSELECTRESULT::JSELECTRESULT(jobject * obj): JOGExpr(obj) { }

JSELECTRESULT*
JSELECTRESULT::fromJObject(jobject *obj)
{
  JSELECTRESULT *_expr = new JSELECTRESULT(obj);
  _expr->setArgs(_expr->generateArgs());
  return _expr;
}

void JSELECTRESULT::debug_print()
{
  printf("JSELECTRESULT class\n");
}
JSELECTRESULT::~JSELECTRESULT() {}


/**
 * C shim to instantiate java tree as c++ tree
 */
#ifdef __cplusplus
extern "C"
#endif
void * instantiateJClassAsCXXClass(jobject obj)
{
  printf("In converter\n");
  convert::ExprFactory * factory = new convert::ExprFactory();
  return factory->getExpr(obj);
}

class DispatchToReal16ArrayOfArrays: public librdag::Visitor
{
  public:
    
    DispatchToReal16ArrayOfArrays(){}
    ~DispatchToReal16ArrayOfArrays()
    {
      real16 ** arr = this->getData();
      if(arr)
      {

        for(int i=0;i<this->getRows();i++)
        {
          delete arr[i];
        }
        delete arr;         
      }
    }
    void visit(librdag::OGExpr SUPPRESS_UNUSED *thing)
    {
        throw convertExcept;
    };
    void visit(librdag::OGArray<real16> *thing)
    {
      this->setData(thing->toReal16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());      
    }
    void visit(librdag::OGArray<complex16> SUPPRESS_UNUSED *thing)
    {
        throw convertExcept;
    }
    void visit(librdag::OGMatrix<real16> *thing)
    {
      this->setData(thing->toReal16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());    
    }
    void visit(librdag::OGMatrix<complex16> SUPPRESS_UNUSED *thing)
    {
        throw convertExcept;
    }    
    void visit(librdag::OGScalar<real16> *thing)
    {
      real16 * const data = thing->getData();
      real16 ** tmp = new real16 * [1];
      tmp[0] = new real16[1];
      tmp[0][0] = *data;
      this->setData(tmp);
      this->setRows(1);
      this->setCols(1);    
    }
    void visit(librdag::OGScalar<complex16> SUPPRESS_UNUSED *thing)
    {
        throw convertExcept;
    }
    void setData(real16 ** data)
    {
      this->_data = data;
    }
    void setRows(int rows)
    {
     this->rows = rows; 
    }
    void setCols(int cols)
    {
     this->cols = cols;
    }        
    real16 ** getData()
    {
      return this->_data;
    }
    int getRows()
    {
     return this->rows; 
    }
    int getCols()
    {
     return this->cols; 
    }    
  private:
    real16 ** _data = NULL;
    int rows;
    int cols;
};

class DispatchToComplex16ArrayOfArrays: public librdag::Visitor
{
  public:
    
    DispatchToComplex16ArrayOfArrays(){}
    ~DispatchToComplex16ArrayOfArrays()
    {
      complex16 ** arr = this->getData();
      if(arr)
      {

        for(int i=0;i<this->getRows();i++)
        {
          delete arr[i];
        }
        delete arr;         
      }
    }
    void visit(librdag::OGExpr SUPPRESS_UNUSED *thing)
    {
        throw convertExcept;
    };
    void visit(librdag::OGArray<complex16> *thing)
    {
      this->setData(thing->toComplex16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());      
    }
    void visit(librdag::OGArray<real16> SUPPRESS_UNUSED *thing)
    {
        throw convertExcept;
    }
    void visit(librdag::OGMatrix<complex16> *thing)
    {
      this->setData(thing->toComplex16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());    
    }
    void visit(librdag::OGMatrix<real16> SUPPRESS_UNUSED *thing)
    {
        throw convertExcept;
    }    
    void visit(librdag::OGScalar<complex16> *thing)
    {
      complex16 * const data = thing->getData();
      complex16 ** tmp = new complex16 * [1];
      tmp[0] = new complex16[1];
      tmp[0][0] = *data;
      this->setData(tmp);
      this->setRows(1);
      this->setCols(1);    
    }
    void visit(librdag::OGScalar<real16> SUPPRESS_UNUSED *thing)
    {
        throw convertExcept;
    }
    void setData(complex16 ** data)
    {
      this->_data = data;
    }
    void setRows(int rows)
    {
     this->rows = rows; 
    }
    void setCols(int cols)
    {
     this->cols = cols;
    }        
    complex16 ** getData()
    {
      return this->_data;
    }
    int getRows()
    {
     return this->rows; 
    }
    int getCols()
    {
     return this->cols; 
    }    
  private:
    complex16 ** _data = NULL;
    int rows;
    int cols;
};


  /**
   * deletes an array of arrays of type T
   */
  template <typename T> void deleteArrOfArr(T ** buf, int lda)
  {
    for(int i=0;i<lda;i++)
    {
      delete buf[i];
    }
    delete buf;    
  }

} // namespace ends

#ifdef __cplusplus
extern "C" {
#endif
 
//  
//
// JNI methods and helpers
//
//  
  
  /*
   * Converts a real16 ** to a java double[][]
   * @param env, the JNI environment pointer
   * @param inputData, the real16 array of arrays to convert
   * @param rows the number of rows in the array
   * @param cols the number of columns in the array
   * @return a jobjectArray which is a double[][] equivalent of {@code inputData}
   */
  jobjectArray convertCreal16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, real16 ** inputData, int rows, int cols)
  {
    jobjectArray returnVal = env->NewObjectArray(rows, BigDDoubleArrayClazz, NULL);
    if(!returnVal)
    {
#ifdef _DEBUG
      printf("Allocation of jobjectArray failed.\n");
#endif
      exit(1);
    }
    for(int i = 0; i < rows; i++)
    {
      jdoubleArray tmp = env->NewDoubleArray(cols);
      env->SetDoubleArrayRegion(tmp, 0, cols, inputData[i]);
      env->SetObjectArrayElement(returnVal, i, tmp);
    }
    return returnVal;
  }



  /**
   * 
   */
  jobjectArray extractRealPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, complex16 ** inputData, int rows, int cols) {
    jobjectArray returnVal = env->NewObjectArray(rows, BigDDoubleArrayClazz, NULL);
    if(!returnVal)
    {
#ifdef _DEBUG
      printf("Allocation of jobjectArray failed.\n");
#endif
      exit(1);
    }
    real16 * aRow = new real16[cols];
    for(int i = 0; i < rows; i++)
    {
      jdoubleArray tmp = env->NewDoubleArray(cols);
      for(int j = 0; j < cols; j++)
      {
        aRow[i]=std::real(inputData[i][j]);
      }
      env->SetDoubleArrayRegion(tmp, 0, cols, aRow);
      env->SetObjectArrayElement(returnVal, i, tmp);
    }
    delete aRow;
    return returnVal;
  }
 
   /**
   * 
   */
  jobjectArray extractImagPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, complex16 ** inputData, int rows, int cols) {
    jobjectArray returnVal = env->NewObjectArray(rows, BigDDoubleArrayClazz, NULL);
    if(!returnVal)
    {
#ifdef _DEBUG
      printf("Allocation of jobjectArray failed.\n");
#endif
      exit(1);
    }
    real16 * aRow = new real16[cols];
    for(int i = 0; i < rows; i++)
    {
      jdoubleArray tmp = env->NewDoubleArray(cols);
      for(int j = 0; j < cols; j++)
      {
        aRow[i]=std::imag(inputData[i][j]);
      }
      env->SetDoubleArrayRegion(tmp, 0, cols, aRow);
      env->SetObjectArrayElement(returnVal, i, tmp);
    }
    delete aRow;
    return returnVal;
  }
 
  /*
   * Class:     com_opengamma_longdog_materialisers_Materialisers
   * Method:    materialiseToJDoubleArrayOfArrays
   * Signature: (Lcom/opengamma/longdog/datacontainers/OGNumeric;)[[D
   */
  JNIEXPORT jobjectArray JNICALL Java_com_opengamma_longdog_materialisers_Materialisers_materialiseToJDoubleArrayOfArrays
  (JNIEnv SUPPRESS_UNUSED *env, jclass SUPPRESS_UNUSED clazz, jobject SUPPRESS_UNUSED obj)
  {
    printf("Entering materialise function\n");

    printf("Calling convert::instantiateJClassAsCXXClass\n");
    // convert obj to OGNumeric objs
    librdag::OGNumeric * chain = (librdag::OGNumeric *) convert::instantiateJClassAsCXXClass(obj);

    printf("Calling entrypt function\n");
    librdag::OGTerminal * answer =  (librdag::OGTerminal *) entrypt((struct c_OGNumeric *) chain);
    
    convert::DispatchToReal16ArrayOfArrays * visitor = new convert::DispatchToReal16ArrayOfArrays();
    answer->accept(*visitor);
    real16 ** buf = visitor->getData();
    int rows = visitor->getRows();;
    int cols = visitor->getCols();
   
    jobjectArray returnVal = convertCreal16ArrOfArr2JDoubleArrOfArr(env, buf, rows, cols);

    delete visitor;
    
    printf("Returning\n");    
    return returnVal;
  }

  /*
   * Class:     com_opengamma_longdog_materialisers_Materialisers
   * Method:    materialiseToJComplexArrayContainer
   * Signature: (Lcom/opengamma/longdog/datacontainers/OGNumeric;)Lcom/opengamma/longdog/datacontainers/other/ComplexArrayContainer;
   */
  JNIEXPORT jobject JNICALL Java_com_opengamma_longdog_materialisers_Materialisers_materialiseToJComplexArrayContainer
  (JNIEnv SUPPRESS_UNUSED *env, jclass SUPPRESS_UNUSED clazz, jobject SUPPRESS_UNUSED obj)
  {
    printf("materialiseToJComplexArrayContainer\n");
    printf("Entering materialise function\n");

    printf("Calling convert::instantiateJClassAsCXXClass\n");
    // convert obj to OGNumeric objs
    librdag::OGNumeric * chain = (librdag::OGNumeric *) convert::instantiateJClassAsCXXClass(obj);

    printf("Calling entrypt function\n");
    librdag::OGTerminal * answer =  (librdag::OGTerminal *) entrypt((struct c_OGNumeric *) chain);
    
    convert::DispatchToComplex16ArrayOfArrays * visitor = new convert::DispatchToComplex16ArrayOfArrays();
    answer->accept(*visitor);
    complex16 ** buf = visitor->getData();
    int rows = visitor->getRows();;
    int cols = visitor->getCols();    
   
    jobjectArray realPart = extractRealPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(env, buf, rows, cols);
    jobjectArray complexPart = extractImagPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(env, buf, rows, cols);

    jobject returnVal = env->NewObject(ComplexArrayContainerClazz,ComplexArrayContainerClazz_ctor_DAoA_DAoA, realPart, complexPart);
    
    delete visitor;
    
    printf("Returning\n");    
    return returnVal;
  }

  /*
   * Class:     com_opengamma_longdog_materialisers_Materialisers
   * Method:    materialiseToJBoolean
   * Signature: (Lcom/opengamma/longdog/datacontainers/OGNumeric;)Z
   */
  JNIEXPORT jboolean JNICALL Java_com_opengamma_longdog_materialisers_Materialisers_materialiseToJBoolean
  (JNIEnv SUPPRESS_UNUSED *env, jclass SUPPRESS_UNUSED clazz, jobject SUPPRESS_UNUSED obj)
  {
    jboolean returnVal = 0;
    return returnVal;
  }


#ifdef __cplusplus
}
#endif
