#include <iostream>
#include <stdexcept>

#include "RNCWebViewLifecycle.h"

#define LOG_TAG "JSIInstaller"

# ifdef ANDROID
// LOGS ANDROID
#   include <android/log.h>
#   define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG,__VA_ARGS__)
#   define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , LOG_TAG,__VA_ARGS__)
#   define LOGI(...) __android_log_print(ANDROID_LOG_INFO   , LOG_TAG,__VA_ARGS__)
#   define LOGW(...) __android_log_print(ANDROID_LOG_WARN   , LOG_TAG,__VA_ARGS__)
#   define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , LOG_TAG,__VA_ARGS__)
#   define LOGSIMPLE(...)
# else
// LOGS NO ANDROID
#   include <stdio.h>
#   define LOGV(...) printf("  ");printf(__VA_ARGS__); printf("\t -  <%s> \n", LOG_TAG);
#   define LOGD(...) printf("  ");printf(__VA_ARGS__); printf("\t -  <%s> \n", LOG_TAG);
#   define LOGI(...) printf("  ");printf(__VA_ARGS__); printf("\t -  <%s> \n", LOG_TAG);
#   define LOGW(...) printf("  * Warning: "); printf(__VA_ARGS__); printf("\t -  <%s> \n", LOG_TAG);
#   define LOGE(...) printf("  *** Error:  ");printf(__VA_ARGS__); printf("\t -  <%s> \n", LOG_TAG);
#   define LOGSIMPLE(...) printf(" ");printf(__VA_ARGS__);
# endif // ANDROID

// global ref to Java Virtual Machine
// used to get JNI current env
JavaVM *jvm;
// global ref to our class instance
static jobject globalObjectRef;
// global ref to our class
static jclass globalClassRef;

//This is how we represent a Java exception already in progress
struct ThrownJavaException : std::runtime_error {
    ThrownJavaException() :std::runtime_error("") {}
    ThrownJavaException(const std::string& msg ) :std::runtime_error(msg) {}
};

inline void assert_no_exception(JNIEnv * env) {
  if (env->ExceptionCheck()==JNI_TRUE)
    throw ThrownJavaException("assert_no_exception");
}

//used to throw a new Java exception. use full paths like:
//"java/lang/NoSuchFieldException"
//"java/lang/NullPointerException"
//"java/security/InvalidParameterException"
struct NewJavaException : public ThrownJavaException{
    NewJavaException(JNIEnv * env, const char* type="", const char* message="")
      :ThrownJavaException(type+std::string(" ")+message)
    {
      jclass newExcCls = env->FindClass(type);
      if (newExcCls != NULL)
        env->ThrowNew(newExcCls, message);
      //if it is null, a NoClassDefFoundError was already thrown
    }
};

void swallow_cpp_exception_and_throw_java(JNIEnv * env) {
  try {
    throw;
  } catch(const ThrownJavaException&) {
    // already reported to Java, ignore
  } catch(const std::bad_alloc& rhs) {
    //  translate OOM C++ exception to a Java exception
    NewJavaException(env, "java/lang/OutOfMemoryError", rhs.what());
  } catch(const std::ios_base::failure& rhs) { //sample translation
    // translate IO C++ exception to a Java exception
    NewJavaException(env, "java/io/IOException", rhs.what());
  } catch(const std::exception& e) {
    // translate unknown C++ exception to a Java exception
    NewJavaException(env, "java/lang/RuntimeException", e.what());
  } catch(...) {
    //  translate unknown C++ exception to a Java exception
    NewJavaException(env, "java/lang/Error", "Unknown exception type");
  }
}

std::string jstring2string(JNIEnv *env, jstring jStr) {
  if (!jStr)
    return "";

  const jclass stringClass = env->GetObjectClass(jStr);
  const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
  const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

  size_t length = (size_t) env->GetArrayLength(stringJbytes);
  jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

  std::string ret = std::string((char *)pBytes, length);
  env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

  env->DeleteLocalRef(stringJbytes);
  env->DeleteLocalRef(stringClass);
  return ret;
}

extern "C" JNIEXPORT void JNICALL
Java_com_reactnativecommunity_webview_jsi_Lifecycle_installBinding(JNIEnv *env, jobject thiz, jlong runtimePtr)
{
  auto &runtime = *(jsi::Runtime *)runtimePtr;
  auto testBinding = std::make_shared<SampleModule>();

  SampleModule::install(runtime, testBinding);

  // storing java vm reference
  env->GetJavaVM(&jvm);

  globalObjectRef = env->NewGlobalRef(thiz);

  auto clazz = env->FindClass("com/reactnativecommunity/webview/jsi/Lifecycle");

  globalClassRef = (jclass)env->NewGlobalRef(clazz);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_reactnativecommunity_webview_jsi_Lifecycle_onShouldStartLoadWithRequest(JNIEnv* env, jobject thiz, jlong runtimePtr, jint viewId, jstring url)
{
  auto &runtime = *(jsi::Runtime *)runtimePtr;
  auto rncWebViewGlobal = runtime.global().getPropertyAsObject(runtime, "RNCWebView");
  std::string functionName = "onShouldStartLoadWithRequest-" + std::to_string(viewId);
//  std::string functionName = "onShouldStartLoadWithRequest";
  auto onShouldStartLoadWithRequest = rncWebViewGlobal.getPropertyAsFunction(runtime, functionName.c_str());
//  return onShouldStartLoadWithRequest.isFunction(runtime);
//  auto utf8url = env->GetStringUTFChars(url, JNI_FALSE);
//  auto urlLen = env->GetStringLength(utf8url);
  try {
    auto result = onShouldStartLoadWithRequest.callWithThis(
      runtime,
      runtime.global(),
      {jsi::String::createFromUtf8(runtime, jstring2string(env, url))});
    return (jboolean)(result.getBool() ? JNI_TRUE : JNI_FALSE);
  } catch (...) {
    swallow_cpp_exception_and_throw_java(env);
    return JNI_FALSE;
  }
}

JNIEnv *attachCurrentThread()
{
  JavaVMAttachArgs args{JNI_VERSION_1_6, nullptr, nullptr};
  JNIEnv *env = nullptr;
  auto result = jvm->AttachCurrentThread(&env, &args);
  return env;
}

void SampleModule::install(
    jsi::Runtime &runtime,
    const std::shared_ptr<SampleModule> sampleModule)
{
  auto testModuleName = "NativeSampleModule";
  auto object = jsi::Object::createFromHostObject(runtime, sampleModule);

  runtime.global().setProperty(runtime, testModuleName, std::move(object));
}

jsi::Value SampleModule::get(
    jsi::Runtime &runtime,
    const jsi::PropNameID &name)
{
  auto methodName = name.utf8(runtime);

  if (methodName == "getMagic") {
    return jsi::Function::createFromHostFunction(
      runtime,
      name,
      0,
      [](
        jsi::Runtime &runtime,
        const jsi::Value &thisValue,
        const jsi::Value *arguments,
        size_t count) -> jsi::Value {
          auto magicNumber = runtime.global().getProperty(runtime, "MagicNumber");
//          auto env = attachCurrentThread();
//
//          auto runTest = env->GetStaticMethodID(globalClassRef, "runTest", "()Ljava/lang/String;");
//          auto str = (jstring)env->CallStaticObjectMethod(globalClassRef, runTest);
//
//          const char *cStr = env->GetStringUTFChars(str, nullptr);
//
//          return jsi::String::createFromAscii(runtime, cStr);
          return magicNumber;
      });
  }

  if (methodName == "getStaticField")
  {
    return jsi::Function::createFromHostFunction(
        runtime,
        name,
        0,
        [](
   jsi::Runtime &runtime,
   const jsi::Value &thisValue,
   const jsi::Value *arguments,
   size_t count) -> jsi::Value {
          auto env = attachCurrentThread();

          auto runTest = env->GetStaticMethodID(globalClassRef, "runTest", "()Ljava/lang/String;");
          auto str = (jstring)env->CallStaticObjectMethod(globalClassRef, runTest);

          const char *cStr = env->GetStringUTFChars(str, nullptr);

          return jsi::String::createFromAscii(runtime, cStr);
        });
  }

  if (methodName == "getStringPrivateField")
  {
    return jsi::Function::createFromHostFunction(
        runtime,
        name,
        0,
        [](
   jsi::Runtime &runtime,
   const jsi::Value &thisValue,
   const jsi::Value *arguments,
   size_t count) -> jsi::Value {
          auto env = attachCurrentThread();

          auto valId = env->GetFieldID(globalClassRef, "stringField", "Ljava/lang/String;");

          auto str = (jstring)env->GetObjectField(globalObjectRef, valId);

          const char *cStr = env->GetStringUTFChars(str, nullptr);

          return jsi::String::createFromAscii(runtime, cStr);
        });
  }

  return jsi::Value::undefined();
}