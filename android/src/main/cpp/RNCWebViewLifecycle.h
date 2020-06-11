#pragma once

#include <jni.h>
#include <jsi/jsi.h>

// TODO: remove this and use specific types from namespace in cpp file
using namespace facebook;

extern "C" {
  JNIEXPORT jboolean JNICALL
  Java_com_reactnativecommunity_webview_jsi_Lifecycle_onShouldStartLoadWithRequest(JNIEnv* env,
    jobject thiz,
    jlong runtimePtr,
    jint viewId,
    jstring url,
    jboolean loading,
    jstring title,
    jboolean canGoBack,
    jboolean canGoForward);
}
