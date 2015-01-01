# macros to include all files with same extension

define all-files-under
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find $(1) -name "$(2)" -and -not -name ".*") \
 )
endef

define all-cpp-files-under
$(call all-files-under,$(1),*.cpp)
endef

define all-c-files-under
$(call all-files-under,$(1),*.c)
endef

# module
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libracing_examples
LOCAL_SRC_FILES := \
	$(call all-cpp-files-under, ./../../../Examples/Source/) \
	$(LOCAL_PATH)/main.cpp \

ENGINE_PATH = $(LOCAL_PATH)/../../../Source/

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/../../../Examples/Source/ \
	$(LOCAL_PATH)/../../../external/bullet/src/ \
	$(ENGINE_PATH)/ \
	$(ENGINE_PATH)/Platform \
	$(ENGINE_PATH)/Render \
	$(ENGINE_PATH)/Scene \
	$(ENGINE_PATH)/Resource \
	$(ENGINE_PATH)/External \
	$(ENGINE_PATH)/External/universal-tween-engine-cpp/ \

LOCAL_STATIC_LIBRARIES += racing_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)