TEMPLATE = lib

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../common.pri)

SOURCES += \
    $$system("find Animation/ -name '*.cpp'") \
    $$system("find Audio/ -name '*.cpp'") \
    $$system("find Base/ -name '*.cpp'") \
    $$system("find Effect/ -name '*.cpp'") \
    $$system("find External/ -name '*.cpp'") \
    $$system("find FileSystem/ -name '*.cpp'") \
    $$system("find Font/ -name '*.cpp'") \
    $$system("find Math/ -name '*.cpp'") \
    $$system("find Message/ -name '*.cpp'") \
    $$system("find Render/ -name '*.cpp'") \
    $$system("find Resource/ -name '*.cpp'") \
    $$system("find Scene/ -name '*.cpp'") \
    $$system("find UI/ -name '*.cpp'") \
    $$system("find Util/ -name '*.cpp'") \
    $$files(Platform/*.cpp) \

HEADERS += \
    $$system("find Animation/ -name '*.h'") \
    $$system("find Audio/ -name '*.h'") \
    $$system("find Base/ -name '*.h'") \
    $$system("find Effect/ -name '*.h'") \
    $$system("find External/ -name '*.h'") \
    $$system("find FileSystem/ -name '*.h'") \
    $$system("find Font/ -name '*.h'") \
    $$system("find Math/ -name '*.h'") \
    $$system("find Message/ -name '*.h'") \
    $$system("find Render/ -name '*.h'") \
    $$system("find Resource/ -name '*.h'") \
    $$system("find Scene/ -name '*.h'") \
    $$system("find UI/ -name '*.h'") \
    $$system("find Util/ -name '*.h'") \
    $$files(*.h) \
    $$files(Platform/*.h) \
    Base/ECS/ComponentHandle.h

RE_PLATFORM_ANDROID {
    SOURCES += \
        $$system("find Platform/android/ -name '*.cpp'") \

    HEADERS += \
        $$system("find Platform/android/ -name '*.h'") \

} RE_PLATFORM_LINUX | RE_PLATFORM_MAC {
    SOURCES += \
        $$system("find Platform/linux/ -name '*.cpp'") \

    HEADERS += \
        $$system("find Platform/linux/ -name '*.h'") \

    SOURCES += \
        $$system("find Platform/Base/desktop/ -name '*.cpp'") \

    HEADERS += \
        $$system("find Platform/Base/desktop/ -name '*.h'") \
}

INCLUDEPATH += \
    . \
    Platform \
    Render \
    Scene \
    Resource \
    External \
    External/universal-tween-engine-cpp/ \
