/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.maths.helpers;

import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;

/**
 * Function for shipping data from one format to another
 */
public class DenseMemoryManipulation {

  /**
   * Converts a row major double pointer to a column major single pointer
   * @param dataIn a row major double[][] non ragged array
   * @return a column major double[] array
   */
  public static double[] convertRowMajorDoublePointerToColumnMajorSinglePointer(double[][] dataIn) {
    if (MatrixPrimitiveUtils.isRagged(dataIn)) {
      throw new MathsExceptionIllegalArgument("Backing array is ragged");
    }
    int rows = dataIn.length;
    int cols = dataIn[0].length;
    double[] tmp = new double[rows * cols];

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        tmp[j * rows + i] = dataIn[i][j];
      }
    }
    return tmp;
  }

  /**
   * Converts a row major double pointer to a column major single pointer with odd elements set to 0 (for complex type purposes)
   * @param dataIn a row major double[][] non ragged array
   * @return a column major double[] array
   */
  public static double[] convertRowMajorDoublePointerToColumnMajorZeroInterleavedSinglePointer(double[][] dataIn) {
    if (MatrixPrimitiveUtils.isRagged(dataIn)) {
      throw new MathsExceptionIllegalArgument("Backing array is ragged");
    }
    int rows = dataIn.length;
    int cols = dataIn[0].length;
    double[] tmp = new double[2 * rows * cols];

    int count = 0;
    for (int i = 0; i < 2 * rows; i += 2) {
      for (int j = 0; j < cols; j++) {
        tmp[j * 2 * rows + i] = dataIn[count][j];
      }
      count++;
    }
    return tmp;
  }

  /**
   * Converts a row major double pointer to a column major single pointer with even elements set to those in dataIn1 and odd elements set to dataIn2 (for complex type purposes)
   * @param dataIn1 a row major double[][] non ragged array
   * @param dataIn2 a row major double[][] non ragged array 
   * @return a column major double[] array
   */
  public static double[] convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointer(double[][] dataIn1, double[][] dataIn2) {
    if (MatrixPrimitiveUtils.isRagged(dataIn1)) {
      throw new MathsExceptionIllegalArgument("Backing array is ragged");
    }
    if (MatrixPrimitiveUtils.isRagged(dataIn2)) {
      throw new MathsExceptionIllegalArgument("Backing array is ragged");
    }
    int rows = dataIn1.length;
    int cols = dataIn1[0].length;

    if (dataIn2.length != rows) {
      throw new MathsExceptionIllegalArgument("Number of rows in first array (" + rows + ") does not match number of rows in second (" + dataIn2.length + ")");
    }
    if (dataIn2[0].length != cols) {
      throw new MathsExceptionIllegalArgument("Number of columns in first array (" + cols + ") does not match number of columns in second (" + dataIn2[0].length + ")");
    }
    double[] tmp = new double[2 * rows * cols];

    int count = 0;
    for (int i = 0; i < 2 * rows; i += 2) {
      for (int j = 0; j < cols; j++) {
        tmp[j * 2 * rows + i] = dataIn1[count][j];
        tmp[j * 2 * rows + i + 1] = dataIn2[count][j];
      }
      count++;
    }
    return tmp;
  }

  public static double[] convertSinglePointerToZeroInterleavedSinglePointer(double[] dataIn) {
    Catchers.catchNullFromArgList(dataIn, 1);
    final int len = dataIn.length;
    double[] tmp = new double[2 * len];
    int ptr = 0;
    for (int i = 0; i < 2 * len; i += 2) {
      tmp[i] = dataIn[ptr++];
    }
    return tmp;
  }

  public static double[] convertTwoSinglePointersToInterleavedSinglePointer(double[] dataIn1, double[] dataIn2) {
    Catchers.catchNullFromArgList(dataIn1, 1);
    Catchers.catchNullFromArgList(dataIn2, 2);
    final int len = dataIn1.length;
    if (dataIn2.length != len) {
      throw new MathsExceptionIllegalArgument("The lengths of the data provided by dataIn1 and dataIn2 are not the same.");
    }
    double[] tmp = new double[2 * len];
    int ptr = 0;
    for (int i = 0; i < 2 * len; i += 2) {
      tmp[i] = dataIn1[ptr];
      tmp[i + 1] = dataIn2[ptr++];
    }
    return tmp;
  }

  public static void fillArrayWithInterleavedComplexValue(double[] arrayToFill, double[] value) {
    Catchers.catchNullFromArgList(arrayToFill, 1);
    Catchers.catchNullFromArgList(value, 2);
    Catchers.catchCondition(arrayToFill.length % 2 != 0, "Array must have even length");
    Catchers.catchCondition(value.length != 2, "value must be double[] length 2, first element real, second imaginary.");
    int n = arrayToFill.length;
    for (int i = 0; i < n; i += 2) {
      arrayToFill[i] = value[0];
      arrayToFill[i + 1] = value[1];
    }
  }
}
