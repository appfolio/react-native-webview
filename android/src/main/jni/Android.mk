LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/common.mk

REACT_NATIVE := $(call find-node-module,$(LOCAL_PATH),react-native)

LOCAL_CPPFLAGS := -std=c++17
LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti
LOCAL_CPPFLAGS += -Wall
LOCAL_CPPFLAGS += -Wextra
LOCAL_CPPFLAGS += -Werror

LOCAL_DISABLE_FATAL_LINKER_WARNINGS := true
LOCAL_LDLIBS := -llog

LOCAL_LDLIBS += /Users/tyler.coffman/Desktop/react-native-0.62.2/jni/x86
LOCAL_LDLIBS += /Users/tyler.coffman/Desktop/react-native-0.62.2/jni/x86_64
LOCAL_LDLIBS += /Users/tyler.coffman/Desktop/react-native-0.62.2/jni/armeabi-v7a
LOCAL_LDLIBS += /Users/tyler.coffman/Desktop/react-native-0.62.2/jni/arm64-v8a

LOCAL_LDLIBS += -ljscexecutor

LOCAL_C_INCLUDES := $(REACT_NATIVE)/ReactCommon/jsi $(LOCAL_PATH)/../cpp

# LOCAL_SRC_FILES += $(REACT_NATIVE)/ReactCommon/jsi/jsi/jsi.cpp
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/../cpp/*.cpp)

LOCAL_MODULE := reactnativecommunity-webview

include $(BUILD_SHARED_LIBRARY)
