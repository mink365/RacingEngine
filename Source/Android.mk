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

# macro
LOCAL_CFLAGS    := -DRE_PLATFORM_ANDROID

# module

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := racing_static

LOCAL_SRC_FILES := \
	$(call all-cpp-files-under, ./Animation/) \
	$(call all-cpp-files-under, ./Audio/) \
	$(call all-cpp-files-under, ./Base/) \
	$(call all-cpp-files-under, ./Effect/) \
	$(call all-cpp-files-under, ./External/) \
	$(call all-cpp-files-under, ./FileSystem/) \
	$(call all-cpp-files-under, ./Font/) \
	$(call all-cpp-files-under, ./Math/) \
	$(call all-cpp-files-under, ./Message/) \
	$(call all-cpp-files-under, ./Render/) \
	$(call all-cpp-files-under, ./Resource/) \
	$(call all-cpp-files-under, ./Scene/) \
	$(call all-cpp-files-under, ./UI/) \
	$(call all-cpp-files-under, ./Util/) \
	$(LOCAL_PATH)/Platform/GameHub.cpp \
	$(call all-cpp-files-under, ./Platform/android/) \

LOCAL_EXPORT_C_INCLUDES := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/Platform \
	$(LOCAL_PATH)/Render \
	$(LOCAL_PATH)/Scene \
	$(LOCAL_PATH)/Resource \
	$(LOCAL_PATH)/External \
	$(LOCAL_PATH)/External/universal-tween-engine-cpp/ \
	$(LOCAL_PATH)/Platform/android/ \

LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES)

# LOCAL_CFLAGS    := -Werror
LOCAL_CPPFLAGS 	:= -Wno-extern-c-compat
LOCAL_EXPORT_CPPFLAGS := -Wno-extern-c-compat

LOCAL_EXPORT_LDLIBS := -llog \
					   -lz \
					   -landroid \
					   -lGLESv2 \
					   -lEGL \
					   -lOpenSLES

EXTERANL_PATH = $(LOCAL_PATH)/../external/
INCLUDE_EXT = include/android/
LIB_EXT = prebuilt/android/$(TARGET_ARCH_ABI)/

LOCAL_C_INCLUDES += \
	$(EXTERANL_PATH)/jpeg/$(INCLUDE_EXT) \
	$(EXTERANL_PATH)/png/$(INCLUDE_EXT) \
	$(EXTERANL_PATH)/freetype2/$(INCLUDE_EXT) \
	$(EXTERANL_PATH)/freetype2/$(INCLUDE_EXT)/freetype2/ \
	$(EXTERANL_PATH)/oggvorbis/include/ \
	$(EXTERANL_PATH)/openal/include/ \

LOCAL_STATIC_LIBRARIES := \
	android_native_app_glue \
	cocos_jpeg_static \
	cocos_png_static \
	cocos_freetype2_static \
	bullet \
	OpenAL_static \
	vorbis_static \

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,jpeg/prebuilt/android)
$(call import-module,png/prebuilt/android)
$(call import-module,freetype2/prebuilt/android)
$(call import-module,oggvorbis/lib/android)
$(call import-module,openal/lib/android)
$(call import-module,bullet)