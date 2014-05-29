TEMPLATE = app

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

LIBS += -lpng -ljpeg
LIBS += -lGL -lGLEW
LIBS += -lglut -lGLU

# 控制defined
DEFINES += _LINUX

SOURCES += \
    $$system("find Source/ -name '*.cpp'") \
#    $$system("find Examples/ -name '*.cpp'") \
    Examples/Source/fbx_render_test.cpp

#SOURCES -= \
#    Source/Animation/Animation.cpp

HEADERS += \
#    $$files(Source/*.h) \
    $$system("find Source/ -name '*.h'") \
    $$system("find Examples/ -name '*.h'") \

INCLUDEPATH += \
    Source/
