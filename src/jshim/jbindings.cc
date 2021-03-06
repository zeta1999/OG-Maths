/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "debug.h"
#include "modifiermacros.h"
#include "jbindings.hh"
#include "jvmmanager.hh"
#include "exceptions.hh"

namespace convert {

/**
 * An adaptor that allows calling Get<type>ArrayElements in a template-parameterised way.
 *
 * @param nativeT the native type of the data that will be obtained
 * @param javaT the Java type of the data
 * @return the pointer to native data from Get<type>ArrayElements
 */
template<typename nativeT, typename javaT>
nativeT* getArrayFromJava(JNIEnv* env, javaT arr);

template<typename nativeT = real8, typename javaT = jdoubleArray>
real8* getArrayFromJava(JNIEnv *env, jdoubleArray arr)
{
  real8* p = env->GetDoubleArrayElements(arr, NULL);
  checkEx(env);
  return p;
}

template<typename nativeT = jint, typename javaT = jintArray>
jint* getArrayFromJava(JNIEnv *env, jintArray arr)
{
  jint* p = env->GetIntArrayElements(arr, NULL);
  checkEx(env);
  return p;
}

/**
 * An adaptor that allows calling Release<type>ArrayElements in a template-parameterised way.
 * The mode is always 0, which copies back the native data and frees the native array.
 *
 * @param nativeT the native type of the data that will be obtained
 * @param javaT the Java type of the data
 * @param nativeArr the pointer to native data from Get<type>ArrayElements
 * @param arr the Java array from which nativeArr was obtained
 */
template<typename nativeT, typename javaT>
void releaseArrayFromJava(JNIEnv* env, nativeT* nativeArr, javaT arr);

template<>
void releaseArrayFromJava(JNIEnv* env, real8* nativeArr, jdoubleArray arr)
{
  env->ReleaseDoubleArrayElements(arr, nativeArr, 0);
}

template<>
void releaseArrayFromJava(JNIEnv* env, complex16* nativeArr, jdoubleArray arr)
{
  env->ReleaseDoubleArrayElements(arr, (real8*) nativeArr, 0);
}

template<>
void releaseArrayFromJava(JNIEnv* env, jint* nativeArr, jintArray arr)
{
  env->ReleaseIntArrayElements(arr, nativeArr, 0);
}

/**
 * bindPrimitiveArrayData
 */

template <typename nativeT, typename javaT> DLLEXPORT_C nativeT * bindPrimitiveArrayData(jobject obj, jmethodID method)
{
  if(obj == nullptr)
  {
    throw convert_error("bindPrimitiveArrayData: null obj");
  }
  if(method == nullptr)
  {
    throw convert_error("bindPrimitiveArrayData: null method");
  }
  VAL64BIT_PRINT("Binding for jobject", obj);
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMManager::getJVM()->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    throw convert_error("Thread attach failed");
  }
  jobject dataobj = NULL;
  dataobj = env->CallObjectMethod(obj, method);
  checkEx(env);
  javaT * array = reinterpret_cast<javaT *>(&dataobj);
  nativeT * _dataptr = (nativeT*) getArrayFromJava<nativeT, javaT>(env, *array);
  return _dataptr;
}

template real8*
bindPrimitiveArrayData<real8, jdoubleArray>(jobject obj, jmethodID method);

template complex16*
bindPrimitiveArrayData<complex16, jdoubleArray>(jobject obj, jmethodID method);

template jint*
bindPrimitiveArrayData<jint, jintArray>(jobject obj, jmethodID method);

/**
 * unbindPrimitiveArrayData
 */

template <typename nativeT, typename javaT> DLLEXPORT_C void unbindPrimitiveArrayData(nativeT * nativeData, jobject obj, jmethodID method)
{
  if(nativeData == nullptr)
  {
    throw convert_error("unbindPrimitiveArrayData: null nativeData");
  }
  if(obj == nullptr)
  {
    throw convert_error("unbindPrimitiveArrayData: null obj");
  }
  if(method == nullptr)
  {
    throw convert_error("unbindPrimitiveArrayData: null method");
  }
  VAL64BIT_PRINT("Unbinding for jobject", obj);
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMManager::getJVM()->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    throw convert_error("Thread attach failed");
  }
  jobject dataobj = env->CallObjectMethod(obj, method);
  checkEx(env);
  javaT * array = reinterpret_cast<javaT *>(&dataobj);
  releaseArrayFromJava<nativeT, javaT>(env, nativeData, *array);
}

template void
unbindPrimitiveArrayData<real8, jdoubleArray>(real8* nativeData, jobject obj, jmethodID method);

template void
unbindPrimitiveArrayData<complex16, jdoubleArray>(complex16* nativeData, jobject obj, jmethodID method);

template void
unbindPrimitiveArrayData<jint, jintArray>(jint* nativeData, jobject obj, jmethodID method);

/**
 * unbindOGArrayData
 */

template <typename nativeT, typename javaT> void unbindOGArrayData(nativeT * nativeData, jobject obj)
{
  unbindPrimitiveArrayData<nativeT, javaT>(nativeData, obj, JVMManager::getOGTerminalClazz_getData());
}

template void
unbindOGArrayData<real8, jdoubleArray>(real8* nativeData, jobject obj);

template void
unbindOGArrayData<complex16, jdoubleArray>(complex16* nativeData, jobject obj);

template void
unbindOGArrayData<jint, jintArray>(jint* nativeData, jobject obj);

} //namespace convert
