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

LOCAL_PATH := $(call my-dir)/../../Source

include $(CLEAR_VARS)

LOCAL_MODULE := libracing
LOCAL_SRC_FILES := $(call all-c-files-under,.) \
	$(call all-cpp-files-under,.)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
	$(LOCAL_PATH)/Platform \
	$(LOCAL_PATH)/Render \
	$(LOCAL_PATH)/Scene \
	$(LOCAL_PATH)/Resource \
	$(LOCAL_PATH)/External/universal-tween-engine-cpp/ \

LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_CFLAGS    := -Werror
LOCAL_LDLIBS    := -llog -landroid -lGLESv2 -lEGL -lGLESv1_CM

EXTERANL_PATH = $(LOCAL_PATH)/../build/external/
INCLUDE_EXT = include/android/
LIB_EXT = prebuilt/android/armeabi-v7a/

LOCAL_C_INCLUDES += \
	$(EXTERANL_PATH)/jpeg/$(INCLUDE_EXT) \
	$(EXTERANL_PATH)/png/$(INCLUDE_EXT) \
	$(EXTERANL_PATH)/freetype2/$(INCLUDE_EXT) \
	$(EXTERANL_PATH)/freetype2/$(INCLUDE_EXT)/freetype2/ \

LOCAL_LDLIBS	+= \
	-L$(EXTERANL_PATH)/jpeg/$(LIB_EXT) \
	-L$(EXTERANL_PATH)/png/$(LIB_EXT) \
	-L$(EXTERANL_PATH)/freetype2/$(LIB_EXT) \

LOCAL_STATIC_LIBRARIES := \
	android_native_app_glue \
	libjpeg \
	libpng \
	libfreetype \

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)