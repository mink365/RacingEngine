
DEPENDPATH += $$PWD/

LIBS += -lSource

INCLUDEPATH += \
    $$PWD/ \
    $$PWD/Platform \
    $$PWD/Render \
    $$PWD/Scene \
    $$PWD/Resource \
    $$PWD/External/universal-tween-engine-cpp/ \

RE_PLATFORM_MAC {
    INCLUDEPATH += $$PWD/Platform/linux/

} RE_PLATFORM_LINUX {
    INCLUDEPATH += $$PWD/Platform/linux/
} RE_PLATFORM_ANDROID {
    INCLUDEPATH += $$PWD/Platform/android/
}

LIBS += -ljpeg
LIBS += -lpng
LIBS += -lGLEW
LIBS += -lfreetype

RE_PLATFORM_MAC {
    LIBS += -L/usr/local/lib

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

unix {
	LIBS += -L$$OUT_PWD/../Source/
#        PRE_TARGETDEPS += $$OUT_PWD/../Source/Source.a
}
