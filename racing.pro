TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

LIBS += -lpng -ljpeg
LIBS += -lGLEW
LIBS += -lfreetype

#CONFIG += RE_PLATFORM_ANDROID
## macros in source code
#DEFINES += RE_PLATFORM_ANDROID

macx {
    CONFIG += RE_PLATFORM_MAC
    DEFINES += RE_PLATFORM_MAC
} else {
    CONFIG += RE_PLATFORM_LINUX
    DEFINES += RE_PLATFORM_LINUX
}

RE_PLATFORM_MAC {
    LIBS += -lglfw3
    LIBS += -framework OpenGL
}
RE_PLATFORM_LINUX {
    LIBS += -lglfw
    LIBS += -lGL

    LIBS += -lBulletCollision \
            -lBulletDynamics \
            -lLinearMath \
            -lConvexDecomposition
}

SOURCES += \
    $$system("find Source/Animation/ -name '*.cpp'") \
    $$system("find Source/Base/ -name '*.cpp'") \
    $$system("find Source/Effect/ -name '*.cpp'") \
    $$system("find Source/External/ -name '*.cpp'") \
    $$system("find Source/FileSystem/ -name '*.cpp'") \
    $$system("find Source/Font/ -name '*.cpp'") \
    $$system("find Source/Math/ -name '*.cpp'") \
    $$system("find Source/Message/ -name '*.cpp'") \
    $$system("find Source/Render/ -name '*.cpp'") \
    $$system("find Source/Resource/ -name '*.cpp'") \
    $$system("find Source/Scene/ -name '*.cpp'") \
    $$system("find Source/UI/ -name '*.cpp'") \
    $$system("find Source/Util/ -name '*.cpp'") \
    $$files(Source/Platform/*.cpp) \
    $$system("find Examples/ -name '*.cpp'") \

HEADERS += \
    $$system("find Source/Animation/ -name '*.h'") \
    $$system("find Source/Base/ -name '*.h'") \
    $$system("find Source/Effect/ -name '*.h'") \
    $$system("find Source/External/ -name '*.h'") \
    $$system("find Source/FileSystem/ -name '*.h'") \
    $$system("find Source/Font/ -name '*.h'") \
    $$system("find Source/Math/ -name '*.h'") \
    $$system("find Source/Message/ -name '*.h'") \
    $$system("find Source/Render/ -name '*.h'") \
    $$system("find Source/Resource/ -name '*.h'") \
    $$system("find Source/Scene/ -name '*.h'") \
    $$system("find Source/UI/ -name '*.h'") \
    $$system("find Source/Util/ -name '*.h'") \
    $$files(Source/*.h) \
    $$files(Source/Platform/*.h) \
    $$system("find Examples/ -name '*.h'") \

#HEADERS -= \
#    $$system("find Source/Platform/ -name '*.h'") \

RE_PLATFORM_ANDROID {
    SOURCES += \
        $$system("find Source/Platform/android/ -name '*.cpp'") \

    HEADERS += \
        $$system("find Source/Platform/android/ -name '*.h'") \

} RE_PLATFORM_LINUX | RE_PLATFORM_MAC {
    SOURCES += \
        $$system("find Source/Platform/linux/ -name '*.cpp'") \

    HEADERS += \
        $$system("find Source/Platform/linux/ -name '*.h'") \

    SOURCES += \
        $$system("find Source/Platform/Base/desktop/ -name '*.cpp'") \

    HEADERS += \
        $$system("find Source/Platform/Base/desktop/ -name '*.h'") \
}

INCLUDEPATH += \
    Source/ \
    Source/Platform \
    Source/Render \
    Source/Scene \
    Source/Resource \
    Source/External/universal-tween-engine-cpp/ \
    Examples/Source/ \

RE_PLATFORM_MAC {
    LIBS += -L/usr/local/lib

    INCLUDEPATH += /usr/local/include/ \
                    /usr/local/include/freetype2/
}
RE_PLATFORM_LINUX {
    INCLUDEPATH += /usr/include/freetype2/
    INCLUDEPATH += /usr/include/bullet/
}
RE_PLATFORM_ANDROID {
    INCLUDEPATH += $$(NDK_ROOT)/platforms/android-18/arch-arm/usr/include/
    INCLUDEPATH += $$(NDK_ROOT)/sources/android/native_app_glue/
}

