LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := vorbis_static
LOCAL_MODULE_FILENAME := vorbis
LOCAL_SRC_FILES := arm/libvorbis.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include/
include $(PREBUILT_STATIC_LIBRARY)