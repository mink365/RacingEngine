unix: CONFIG +=c++11

unix: QMAKE_CXXFLAGS += -std=c++11

#CONFIG += RE_PLATFORM_ANDROID
## macros in source code
#DEFINES += RE_PLATFORM_ANDROID

macx {
    unix: QMAKE_CXXFLAGS = -mmacosx-version-min=10.6

    CONFIG += RE_PLATFORM_MAC
    DEFINES += RE_PLATFORM_MAC
} else {
    CONFIG += RE_PLATFORM_LINUX
    DEFINES += RE_PLATFORM_LINUX
}

RE_PLATFORM_MAC {
    INCLUDEPATH += /usr/local/include/ \
                    /usr/local/include/freetype2/ \
                    /usr/local/include/bullet/
} RE_PLATFORM_LINUX {
    INCLUDEPATH += /usr/include/freetype2/
    INCLUDEPATH += /usr/include/bullet/
} RE_PLATFORM_ANDROID {
    INCLUDEPATH += $$(NDK_ROOT)/platforms/android-18/arch-arm/usr/include/
    INCLUDEPATH += $$(NDK_ROOT)/sources/android/native_app_glue/
}
