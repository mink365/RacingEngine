CONFIG += c++11

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
DEFINES += FMT_HEADER_ONLY

RE_PLATFORM_MAC {
    INCLUDEPATH += /usr/local/include/ \
                    /usr/local/include/freetype2/ \
                    /usr/local/include/bullet/

    LIBS += -L/usr/local/lib
} RE_PLATFORM_LINUX {
    INCLUDEPATH += /usr/include/freetype2/
    INCLUDEPATH += /usr/include/bullet/
} RE_PLATFORM_ANDROID {
    INCLUDEPATH += $$(NDK_ROOT)/platforms/android-18/arch-arm/usr/include/
    INCLUDEPATH += $$(NDK_ROOT)/sources/android/native_app_glue/
}

LIBS += -ljpeg
LIBS += -lpng
LIBS += -lGLEW
LIBS += -lfreetype

LIBS += -lopenal
LIBS += -lvorbis -lvorbisenc -lvorbisfile -logg

RE_PLATFORM_MAC {
    LIBS += -lglfw3
    LIBS += -framework OpenGL

    LIBS += -lBulletCollision \
            -lBulletDynamics \
            -lLinearMath \

} RE_PLATFORM_LINUX {
    LIBS += -lglfw
    LIBS += -lGL

    LIBS += -lBulletCollision \
            -lBulletDynamics \
            -lLinearMath \
            -lConvexDecomposition
}

win32 {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Source/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Source/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Source/release/Source.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Source/debug/Source.lib
}
