TEMPLATE = app

include(../common.pri)
include(../Source/Source.pri)

SOURCES += \
    $$system("find Source/ -name '*.cpp'") \

HEADERS += \
    $$system("find Source/ -name '*.h'") \

INCLUDEPATH += \
    Source/ \

RE_PLATFORM_MAC {
    SOURCES     += ../build/desktop/main.cpp
} RE_PLATFORM_LINUX {
    SOURCES     += ../build/desktop/main.cpp
} RE_PLATFORM_ANDROID {
    SOURCES     += ../build/android/jni/main.cpp
}
