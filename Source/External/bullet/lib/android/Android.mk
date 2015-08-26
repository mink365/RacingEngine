LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := bullet_static
LOCAL_MODULE_FILENAME := bullet
LOCAL_SRC_FILES := arm/libbullet.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include/
include $(PREBUILT_STATIC_LIBRARY)