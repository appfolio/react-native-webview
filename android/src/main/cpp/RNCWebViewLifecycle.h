#pragma once

#include <jni.h>
#include <jsi/jsi.h>

using namespace facebook;

extern "C" {
  JNIEXPORT void JNICALL
  Java_com_reactnativecommunity_webview_jsi_JSIInstaller_installBinding(JNIEnv* env, jobject thiz, jlong runtimePtr);

  JNIEXPORT jboolean JNICALL
  Java_com_reactnativecommunity_webview_jsi_JSIInstaller_shouldOverrideUrlLoading(JNIEnv* env, jobject thiz, jlong runtimePtr, jint viewId, jstring url);
}

class SampleModule : public jsi::HostObject {
public:
    static void install(
        jsi::Runtime &runtime,
        const std::shared_ptr<SampleModule> sampleModule
    );

//    SampleModule(JNIEnv *jniEnv);

    jsi::Value get(jsi::Runtime &runtime, const jsi::PropNameID &name) override;
private:
    JNIEnv jniEnv_;
};