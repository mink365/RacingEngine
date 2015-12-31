# - Try to find OpenAL
# Once done this will define
# OPENAL_INCLUDE_DIRS, where to find al.h, etc.
# OPENAL_LIBRARIES, the libraries to link against to use OpenAL.
# OPENAL_FOUND, If false, do not try to use OpenAL.

if(ANDROID)
    find_path(OPENAL_INCLUDE_DIRS AL/al.h
            PATHS
            "${RE_EXTERNAL_DIR}/openal/include"
            NO_DEFAULT_PATH
            )

    find_library(OPENAL_LIBRARIES
            NAMES OpenAL
            PATHS
            "${RE_EXTERNAL_DIR}/openal/lib/android/arm"
            NO_DEFAULT_PATH
            )
elseif(IOS)
    FIND_PATH(OPENAL_INCLUDE_DIRS AL/al.h
            PATHS
            "${RE_EXTERNAL_DIR}/openal/include"
            NO_DEFAULT_PATH
            )

    find_library(OPENAL_LIBRARIES
            NAMES OpenAL
            )
else()
    find_path(OPENAL_INCLUDE_DIRS AL/al.h
            PATHS
            "${RE_EXTERNAL_DIR}/openal/include"
            NO_DEFAULT_PATH
            )

    find_library(OPENAL_LIBRARIES
            NAMES OpenAL
            )
endif()

if(OPENAL_INCLUDE_DIRS)
    set(OPENAL_FOUND TRUE)
    MESSAGE(STATUS "Found OpenAL's include: ${OPENAL_INCLUDE_DIRS} lib: ${OPENAL_LIBRARIES}")
else()
    MESSAGE(STATUS "Could NOT find OpenAL.")
endif()