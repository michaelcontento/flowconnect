LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := \
    hellocpp/main.cpp \
    $(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)) \
    $(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/../../Classes/*/*.cpp))

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
#LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
#LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += avalon_static

# enable c++11 support but "remove" the override specifier with a simple
# preprocessor define - it's not supported yet :(
LOCAL_CFLAGS += -std=c++11 -Doverride=

AVALON_FEATURES := ads payment
AVALON_FLAVOR := google

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
#$(call import-module,external/Box2D)
#$(call import-module,external/chipmunk)
$(call import-module,projects/flowconnect/Vendors/avalon)
