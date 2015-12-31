# - Try to find JPEG
# Once done this will define
# JPEG_INCLUDE_DIRS, where to find jpeglib.h, etc.
# JPEG_LIBRARIES, the libraries needed to use JPEG.
# JPEG_FOUND, If false, do not try to use JPEG.

if(ANDROID)
    find_path(JPEG_INCLUDE_DIRS jpeglib.h
            PATHS
            "${RE_EXTERNAL_DIR}/jpeg/include/android"
            NO_DEFAULT_PATH
            )

    find_library(JPEG_LIBRARIES
            NAMES jpeg libjpeg
            PATHS
            "${RE_EXTERNAL_DIR}/jpeg/prebuilt/android/${ANDROID_ABI}"
            NO_DEFAULT_PATH
            )

elseif(IOS)
    FIND_PATH(JPEG_INCLUDE_DIRS jpeglib.h
            PATHS
            "${RE_EXTERNAL_DIR}/jpeg/include/ios"
            NO_DEFAULT_PATH
            )

    find_library(JPEG_LIBRARIES
            NAMES jpeg libjpeg
            PATHS
            "${RE_EXTERNAL_DIR}/jpeg/prebuilt/ios"
            NO_DEFAULT_PATH
            )

else()
    find_path(JPEG_INCLUDE_DIRS jpeglib.h
            PATHS
            "/usr/local/include"
            NO_DEFAULT_PATH
            )

    find_path(JPEG_INCLUDE_DIRS jpeglib.h
            )

    find_library(JPEG_LIBRARIES
            NAMES jpeg libjpeg
            )

endif()

if(JPEG_INCLUDE_DIRS)
    set(JPEG_FOUND TRUE)
    MESSAGE(STATUS "Found JPEG's include: ${JPEG_INCLUDE_DIRS} lib: ${JPEG_LIBRARIES}")
else()
    MESSAGE(STATUS "Could NOT find JPEG.")
endif()
