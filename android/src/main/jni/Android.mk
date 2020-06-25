LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/common.mk

REACT_NATIVE := $(call find-node-module,$(LOCAL_PATH),react-native)

# LOCAL_CPPFLAGS := -std=c++14
LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti
LOCAL_CPPFLAGS += -fvisibility=hidden
# LOCAL_CPPFLAGS += -O3
LOCAL_CPPFLAGS += -Wall
LOCAL_CPPFLAGS += -Wextra
LOCAL_CPPFLAGS += -Werror

LOCAL_LDLIBS := -llog

LOCAL_C_INCLUDES := $(REACT_NATIVE)/ReactCommon/jsi $(LOCAL_PATH)/../cpp

# LOCAL_SRC_FILES += $(REACT_NATIVE)/ReactCommon/jsi/jsi/jsi.cpp
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/../cpp/*.cpp)

LOCAL_SHARED_LIBRARIES := jsireact

LOCAL_MODULE := reactnativecommunity-webview

include $(BUILD_SHARED_LIBRARY)

$(call import-module,jsiexecutor)
$(call import-module,jsi)
# $(call import-module,folly_json)
# $(call import-module,glog)
$(call import-module,reactnative)
