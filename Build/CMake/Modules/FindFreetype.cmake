# - Try to find Freetype
# Once done this will define
# FREETYPE_LIBRARIES, the library to link against
# FREETYPE_FOUND, if false, do not try to link to FREETYPE
# FREETYPE_INCLUDE_DIRS, where to find headers.
# FREETYPE_VERSION_STRING, the version of freetype found (since CMake 2.8.8)

MESSAGE("ANdroid: ${ANDROID}")

if(ANDROID)
    find_path(FREETYPE_INCLUDE_DIRS freetype.h
            PATHS
            "${RE_EXTERNAL_DIR}/freetype2/include/android/freetype2"
            NO_DEFAULT_PATH
            )

    find_library(FREETYPE_LIBRARIES
            NAMES freetype libfreetype
            PATHS
            "${RE_EXTERNAL_DIR}/freetype2/prebuilt/android/${ANDROID_ABI}"
            NO_DEFAULT_PATH
            )
elseif(IOS)
    FIND_PATH(FREETYPE_INCLUDE_DIRS freetype.h
            PATHS
            "${RE_EXTERNAL_DIR}/freetype2/include/ios/freetype2"
            NO_DEFAULT_PATH
            )

    find_library(FREETYPE_LIBRARIES
            NAMES freetype libfreetype
            PATHS
            "${RE_EXTERNAL_DIR}/freetype2/prebuilt/ios"
            NO_DEFAULT_PATH
            )
else()
    find_path(FREETYPE_INCLUDE_DIRS freetype.h
            PATHS
            "/usr/local/include/freetype2"
            )

    find_library(FREETYPE_LIBRARIES
            NAMES freetype
            )
endif()

if(FREETYPE_INCLUDE_DIRS)
    set(FREETYPE_FOUND TRUE)
    MESSAGE(STATUS "Found Freetype's include: ${FREETYPE_INCLUDE_DIRS} lib: ${FREETYPE_LIBRARIES}")
else()
    MESSAGE(STATUS "Could NOT find Freetype.")
endif()