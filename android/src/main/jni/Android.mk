LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
NDK_DEBUG=1
LOCAL_CPPFLAGS := -std=c++17
LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti
LOCAL_CPPFLAGS += -Wall
LOCAL_CPPFLAGS += -Wextra
#LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/../../../../djinni/jni
#LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/../../../../djinni/cpp
#LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/../../../../node_modules/djinni/support-lib/jni
#LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/../../../../node_modules/djinni/support-lib
LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/../../../../../../node_modules/react-native/React
LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/../../../../../../node_modules/react-native/React/Base
LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/../../../../../../node_modules/react-native/ReactCommon/jsi
LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/../../../../../../node_modules/react-native/ReactCommon/jsi
LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/../cpp
#LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/../../../../src/cpp
#LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../djinni/jni/NativeWebviewEvent.cpp
#LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../djinni/jni/NativeWebviewLifecycle.cpp
#LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/../../../../src/cpp/*.cpp)
LOCAL_LDLIBS := -llog
LOCAL_SRC_FILES += $(LOCAL_PATH)/../../../../../../node_modules/react-native/ReactCommon/jsi/jsi/jsi.cpp
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/../cpp/*.cpp)
#LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/../../../../node_modules/djinni/support-lib/jni/*.cpp)
#LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/../../../../node_modules/djinni/support-lib/*.cpp)
LOCAL_MODULE := reactnativecommunity-webview
include $(BUILD_SHARED_LIBRARY)
