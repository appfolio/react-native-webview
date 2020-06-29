LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/common.mk

REACT_NATIVE := $(call find-node-module,$(LOCAL_PATH),react-native)

LOCAL_CPPFLAGS := -std=c++14
LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti
LOCAL_CPPFLAGS += -fvisibility=hidden
LOCAL_CPPFLAGS += -O3
LOCAL_CPPFLAGS += -Wall
LOCAL_CPPFLAGS += -Wextra
# LOCAL_CPPFLAGS += -Werror
LOCAL_CPPFLAGS += -v

# LOCAL_LDLIBS := -llog -L$(REACT_NATIVE)/ReactAndroid/build/tmp/buildReactNdkLib/local/arm64-v8a -ljscexecutor
# LOCAL_LDLIBS := -llog -L/Users/tyler.coffman/src/react-native-webview/node_modules/react-native/ReactAndroid/build/tmp/buildReactNdkLib/local/ -ljscexecutor
LOCAL_DISABLE_FATAL_LINKER_WARNINGS=true
LOCAL_LDLIBS += -llog
# LOCAL_LDLIBS += -L/Users/tyler.coffman/src/react-native-webview/node_modules/react-native/ReactAndroid/build/tmp/buildReactNdkLib/local/arm64-v8a
# LOCAL_LDLIBS += -L/Users/tyler.coffman/src/react-native-webview/node_modules/react-native/ReactAndroid/build/tmp/buildReactNdkLib/local/x86
# LOCAL_LDLIBS += -L/Users/tyler.coffman/src/react-native-webview/node_modules/react-native/ReactAndroid/build/tmp/buildReactNdkLib/local/x86_64
# LOCAL_LDLIBS += -L/Users/tyler.coffman/src/react-native-webview/node_modules/react-native/ReactAndroid/build/tmp/buildReactNdkLib/local/armeabi-v7a/
LOCAL_LDLIBS += -L/Users/tyler.coffman/src/react-native-webview/node_modules/react-native/ReactAndroid/build/intermediates/stripped_native_libs/debug/out/lib/armeabi-v7a/
LOCAL_LDLIBS += -L/Users/tyler.coffman/src/react-native-webview/node_modules/react-native/ReactAndroid/build/intermediates/stripped_native_libs/debug/out/lib/arm64-v8a/
LOCAL_LDLIBS += -L/Users/tyler.coffman/src/react-native-webview/node_modules/react-native/ReactAndroid/build/intermediates/stripped_native_libs/debug/out/lib/x86/
LOCAL_LDLIBS += -L/Users/tyler.coffman/src/react-native-webview/node_modules/react-native/ReactAndroid/build/intermediates/stripped_native_libs/debug/out/lib/x86_64/
LOCAL_LDLIBS += -ljscexecutor


LOCAL_C_INCLUDES := $(REACT_NATIVE)/ReactCommon/jsi $(LOCAL_PATH)/../cpp

# LOCAL_SRC_FILES += $(REACT_NATIVE)/ReactCommon/jsi/jsi/jsi.cpp
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/../cpp/*.cpp)

LOCAL_MODULE := reactnativecommunity-webview

include $(BUILD_SHARED_LIBRARY)
