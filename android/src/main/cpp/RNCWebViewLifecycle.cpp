#include <iostream>
#include <stdexcept>

#include "RNCWebViewLifecycle.h"

#define LOG_TAG "RNCWebViewLifecycle"

#define UNUSED(x) (void)(x);

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

// This is how we represent a Java exception already in progress
struct ThrownJavaException : std::runtime_error {
    ThrownJavaException() :std::runtime_error("") {}
    ThrownJavaException(const std::string& msg ) :std::runtime_error(msg) {}
};

inline void assert_no_exception(JNIEnv * env) {
  if (env->ExceptionCheck()==JNI_TRUE)
    throw ThrownJavaException("assert_no_exception");
}

// used to throw a new Java exception. use full paths like:
// "java/lang/NoSuchFieldException"
// "java/lang/NullPointerException"
// "java/security/InvalidParameterException"
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
  } catch(const jsi::JSError& e) {
    NewJavaException(env, "java/lang/Exception", e.what());
  } catch(const ThrownJavaException&) {
    // already reported to Java, ignore
  } catch(const std::bad_alloc& rhs) {
    // translate OOM C++ exception to a Java exception
    NewJavaException(env, "java/lang/OutOfMemoryError", rhs.what());
  } catch(const std::ios_base::failure& rhs) { //sample translation
    // translate IO C++ exception to a Java exception
    NewJavaException(env, "java/io/IOException", rhs.what());
  } catch(const std::exception& e) {
    // translate unknown C++ exception to a Java exception
    NewJavaException(env, "java/lang/RuntimeException", e.what());
  } catch(...) {
    // translate unknown C++ exception to a Java exception
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

extern "C" JNIEXPORT jboolean JNICALL
Java_com_reactnativecommunity_webview_jsi_Lifecycle_onShouldStartLoadWithRequest(
  JNIEnv* env,
  jobject thiz,
  jlong runtimePtr,
  jint viewId,
  jstring url,
  jboolean loading,
  jstring title,
  jboolean canGoBack,
  jboolean canGoForward)
{
  UNUSED(thiz);

  auto *runtime = (jsi::Runtime *)runtimePtr; // TODO: use a c++ style cast
  if (runtime != nullptr) {
    auto rncWebViewGlobal = runtime->global().getPropertyAsObject(*runtime, "RNCWebView");
    std::string functionName = "onShouldStartLoadWithRequest-" + std::to_string(viewId);
    auto onShouldStartLoadWithRequest = rncWebViewGlobal.getPropertyAsFunction(*runtime, functionName.c_str());
    try {
      auto nativeEvent = jsi::Object(*runtime);
      nativeEvent.setProperty(*runtime, "url", jstring2string(env, url));
      nativeEvent.setProperty(*runtime, "title", jstring2string(env, title));
      nativeEvent.setProperty(*runtime, "loading", (bool)(loading == JNI_TRUE));
      nativeEvent.setProperty(*runtime, "canGoBack", (bool)(canGoBack == JNI_TRUE));
      nativeEvent.setProperty(*runtime, "canGoForward", (bool)(canGoForward == JNI_TRUE));

      auto event = jsi::Object(*runtime);
      event.setProperty(*runtime, "nativeEvent", nativeEvent);

      auto result = onShouldStartLoadWithRequest.call(*runtime, event);
      return (jboolean)(result.getBool() ? JNI_TRUE : JNI_FALSE);
    } catch (...) {
      swallow_cpp_exception_and_throw_java(env);
      return JNI_FALSE;
    }
  } else {
    LOGE("Unexpected null JSI runtime found! Falling back to allowing request.");
    return JNI_FALSE;
  }
}
