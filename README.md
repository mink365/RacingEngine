RacingEngine
============

for MAC:
------------

```sh
brew install glfw3 freetype glew jpeg libpng bullet openal-soft vorbis
```

Qt5.5 support c++14 has a bug, so we need to modify the `Qt/5.5/clang_64/mkspecs/features/c++14.prf`
add this:

```qmake
contains(QMAKE_LFLAGS_CXX11, -stdlib=libc++) {
    QMAKE_CXXFLAGS_CXX11 += -stdlib=libc++
}
```

for Android:
-------------

```bash
export ANDROID_NDK=/absolute/path/to/the/android-ndk
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../Build/CMake/Modules/android.toolchain.cmake -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-clang3.6 -DANDROID_STL=c++_static -DANDROID_NATIVE_API_LEVEL=android-9 ../
make Examples
```

we need to use the `c++_static` because of the bug of `cmath` when we use `NDK` with `gnustl_static`:

[C++11 cmath functions not in std namespace for android NDK w/gcc-4.8 or clang 3.4](http://stackoverflow.com/questions/22922961/c11-cmath-functions-not-in-std-namespace-for-android-ndk-w-gcc-4-8-or-clang-3)
