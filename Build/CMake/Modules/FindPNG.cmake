# - Try to find PNG
# Once done this will define
# PNG_INCLUDE_DIRS, where to find png.h, etc.
# PNG_LIBRARIES, the libraries to link against to use PNG.
# PNG_DEFINITIONS - You should add_definitons(${PNG_DEFINITIONS}) before compiling code that includes png library files.
# PNG_FOUND, If false, do not try to use PNG.
# PNG_VERSION_STRING - the version of the PNG library found (since CMake 2.8.8)

if(ANDROID)
    find_path(PNG_INCLUDE_DIRS png.h
            PATHS
            "${RE_EXTERNAL_DIR}/png/include/android"
            NO_DEFAULT_PATH
            )

    find_library(PNG_LIBRARIES
            NAMES png libpng
            PATHS
            "${RE_EXTERNAL_DIR}/png/prebuilt/android/${ANDROID_ABI}"
            NO_DEFAULT_PATH
            )

elseif(IOS)
    FIND_PATH(PNG_INCLUDE_DIRS png.h
            PATHS
            "${RE_EXTERNAL_DIR}/png/include/ios"
            NO_DEFAULT_PATH
            )

    find_library(PNG_LIBRARIES
            NAMES png libpng
            PATHS
            "${RE_EXTERNAL_DIR}/png/prebuilt/ios"
            NO_DEFAULT_PATH
            )

else()
    find_path(PNG_INCLUDE_DIRS png.h
            PATHS
            "/usr/local/include"
            NO_DEFAULT_PATH
            )

    find_path(PNG_INCLUDE_DIRS png.h
            )

    find_library(PNG_LIBRARIES
            NAMES png libpng
            )

endif()

if(PNG_INCLUDE_DIRS)
    set(PNG_FOUND TRUE)
    MESSAGE(STATUS "Found PNG's include: ${PNG_INCLUDE_DIRS} lib: ${PNG_LIBRARIES}")
else()
    MESSAGE(STATUS "Could NOT find PNG.")
endif()
