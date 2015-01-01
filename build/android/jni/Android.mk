
include files_under.mk

# module
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libracing_examples

LOCAL_SRC_FILES := \
	$(call all-cpp-files-under, ./../../../Examples/Source/) \
	$(LOCAL_PATH)/main.cpp \

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/../../../Examples/Source/ \

LOCAL_STATIC_LIBRARIES += racing_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)