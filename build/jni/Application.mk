NDK_TOOLCHAIN_VERSION=clang
APP_STL := gnustl_static
APP_ABI := armeabi-v7a
APP_CPPFLAGS := -frtti -DRE_PLATFORM_ANDROID=1
APP_CPPFLAGS += -std=c++11
