TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

LIBS += -lpng -ljpeg
LIBS += -lGL -lGLEW -lglfw
LIBS += -lfreetype

# 控制defined
DEFINES += _LINUX

SOURCES += \
    $$system("find Source/ -name '*.cpp'") \
#    $$system("find Examples/ -name '*.cpp'") \
    Examples/Source/fbx_render_test.cpp \
    Examples/Source/main.cpp \

SOURCES -= \
#    Source/Animation/Animation.cpp
#    $$system("find Source/External/ -name '*.cpp'") \

HEADERS += \
#    $$files(Source/*.h) \
    $$system("find Source/ -name '*.h'") \
    $$system("find Examples/ -name '*.h'") \

INCLUDEPATH += \
    Source/ \
    Source/External/universal-tween-engine-cpp/ \

INCLUDEPATH += "/usr/include/freetype2/"
