RacingEngine
============

for MAC:
------------

    brew install glfw3 freetype glew jpeg libpng bullet openal-soft vorbis

    Qt5.5 support c++14 has a bug, so we need to modify the Qt/5.5/clang_64/mkspecs/features/c++14.prf
    add this:
    contains(QMAKE_LFLAGS_CXX11, -stdlib=libc++) {
        QMAKE_CXXFLAGS_CXX11 += -stdlib=libc++
    }
