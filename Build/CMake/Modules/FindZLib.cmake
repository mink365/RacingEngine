# - Try to find zlib
# Once done this will define
# ZLIB_FOUND - Was zlib found
# ZLIB_INCLUDE_DIRS - the zlib include directories
# ZLIB_LIBRARIES - the library to link against

if(ANDROID)
    find_path(ZLIB_INCLUDE_DIRS zlib.h
            PATHS
            "${RE_EXTERNAL_DIR}/zlib/include"
            NO_DEFAULT_PATH
            )

    find_library(ZLIB_LIBRARIES
            NAMES z libz
            PATHS
            "${RE_EXTERNAL_DIR}/zlib/prebuilt/android/${ANDROID_ABI}"
            NO_DEFAULT_PATH
            )
else()
    find_path(FREETYPE_INCLUDE_DIRS zlib.h
            PATHS
            "/usr/local/include/"
            )

    find_library(FREETYPE_LIBRARIES
            NAMES z libz
            )
endif()

if(ZLIB_INCLUDE_DIRS)
    set(ZLIB_FOUND TRUE)
    MESSAGE(STATUS "Found zlib's include: ${ZLIB_INCLUDE_DIRS} lib: ${ZLIB_LIBRARIES}")
else()
    MESSAGE(STATUS "Could NOT find zlib.")
endif()