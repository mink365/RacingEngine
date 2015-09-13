
DEPENDPATH += $$PWD/

include(../common.pri)

INCLUDEPATH += \
    $$PWD/ \
    $$PWD/Platform \
    $$PWD/Render \
    $$PWD/Scene \
    $$PWD/Resource \
    $$PWD/External/hana/include/ \
    $$PWD/External/universal-tween-engine-cpp/ \

RE_PLATFORM_MAC {
    INCLUDEPATH += $$PWD/Platform/linux/
} RE_PLATFORM_LINUX {
    INCLUDEPATH += $$PWD/Platform/linux/
} RE_PLATFORM_ANDROID {
    INCLUDEPATH += $$PWD/Platform/android/
}

unix {
    LIBS += -L$$OUT_PWD/../Source/
#    PRE_TARGETDEPS += $$OUT_PWD/../Source/Source.a

    message($$OUT_PWD/../Source/)
}

LIBS += -lSource
