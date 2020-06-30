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
LOCAL_CPPFLAGS += -v

LOCAL_DISABLE_FATAL_LINKER_WARNINGS := true
LOCAL_LDLIBS := -llog

LOCAL_LDLIBS += -L$(BUILD_DIR)/_libraries/jni/$(APP_ABI)

LOCAL_LDLIBS += -v -ljscexecutor

V := 1


# LOCAL_C_INCLUDES := $(REACT_NATIVE)/ReactCommon/jsi $(LOCAL_PATH)/../cpp

# LOCAL_SRC_FILES += $(REACT_NATIVE)/ReactCommon/jsi/jsi/jsi.cpp
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/../cpp/*.cpp)

LOCAL_MODULE := reactnativecommunity-webview

include $(BUILD_SHARED_LIBRARY)
