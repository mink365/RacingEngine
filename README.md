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

for Android:
-------------
	```
	$ export ANDROID_NDK=/absolute/path/to/the/android-ndk
	$ mkdir build && cd build
	$ cmake -DCMAKE_TOOLCHAIN_FILE=../Build/CMake/Modules/android.toolchain.cmake -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-clang3.6 -DANDROID_STL=c++_static -DANDROID_NATIVE_API_LEVEL=android-9 ../
	$ make Examples
	```
