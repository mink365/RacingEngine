TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

LIBS += -lpng -ljpeg
LIBS += -lGLEW -lglfw3
LIBS += -lfreetype

macx {
    LIBS += -framework OpenGL
} else {
    LIBS += -lGL
}

# 控制defined
DEFINES += _LINUX

SOURCES += \
    $$system("find Source/ -name '*.cpp'") \
    $$system("find Examples/ -name '*.cpp'") \

SOURCES -= \
#    Source/Animation/Animation.cpp
#    $$system("find Source/External/ -name '*.cpp'") \

HEADERS += \
#    $$files(Source/*.h) \
    $$system("find Source/ -name '*.h'") \
    $$system("find Examples/ -name '*.h'") \

INCLUDEPATH += \
    Source/ \
    Source/Platform \
    Source/Render \
    Source/Scene \
    Source/Resource \
    Source/External/universal-tween-engine-cpp/ \
    Examples/Source/ \

macx {
    LIBS += -L/usr/local/lib

    INCLUDEPATH += /usr/local/include/ \
                    /usr/local/include/freetype2/
} else {
    INCLUDEPATH += /usr/include/freetype2/
}

