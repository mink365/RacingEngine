# - Try to find Vorbis
# Once done this will define
# VORBIS_INCLUDE_DIRS, where to find vorbisenc.h, etc.
# VORBIS_LIBRARIES, the libraries to link against to use Vorbis.
# VORBIS_FOUND, If false, do not try to use Vorbis.

if(ANDROID)
    find_path(VORBIS_INCLUDE_DIRS ogg/ogg.h
            PATHS
            "${RE_EXTERNAL_DIR}/oggvorbis/include"
            NO_DEFAULT_PATH
            )

    find_library(VORBIS_LIBRARIES
            NAMES vorbis
            PATHS
            "${RE_EXTERNAL_DIR}/oggvorbis/lib/android/arm"
            NO_DEFAULT_PATH
            )
elseif(IOS)
    FIND_PATH(VORBIS_INCLUDE_DIRS ogg/ogg.h
            PATHS
            "${RE_EXTERNAL_DIR}/oggvorbis/include"
            NO_DEFAULT_PATH
            )
    
    find_library(VORBIS_LIBRARIE
            NAMES vorbis
            PATHS
            "${RE_EXTERNAL_DIR}/oggvorbis/lib/ios/${IOS_ARCH}"
            NO_DEFAULT_PATH
            )

    find_library(OGG_LIBRARIE
            NAMES ogg
            PATHS
            "${RE_EXTERNAL_DIR}/oggvorbis/lib/ios/${IOS_ARCH}"
            NO_DEFAULT_PATH
            )

    set(VORBIS_LIBRARIES
            ${VORBIS_LIBRARIE}
            ${OGG_LIBRARIE}
            )
elseif(WIN)
    find_path(VORBIS_INCLUDE_DIRS vorbisenc.h
            )

    find_library(VORBIS_LIBRARIE
            NAMES vorbis
            )

    find_library(VORBIS_FILE_LIBRARIE
            NAMES vorbisfile
            )

    find_library(OGG_LIBRARIE
            NAMES ogg
            )

    set(VORBIS_LIBRARIES
            ${VORBIS_LIBRARIE}
            ${VORBIS_FILE_LIBRARIE}
            ${OGG_LIBRARIE}
            )
else()
    find_path(VORBIS_INCLUDE_DIRS vorbis/vorbisenc.h
            )

    find_library(VORBIS_LIBRARIE
            NAMES vorbis
            )

    find_library(VORBIS_FILE_LIBRARIE
            NAMES vorbisfile
            )

    find_library(OGG_LIBRARIE
            NAMES ogg
            )

    set(VORBIS_LIBRARIES
            ${VORBIS_LIBRARIE}
            ${VORBIS_FILE_LIBRARIE}
            ${OGG_LIBRARIE}
            )
endif()

if(VORBIS_INCLUDE_DIRS)
    set(VORBIS_FOUND TRUE)
    MESSAGE(STATUS "Found Vorbis's include: ${VORBIS_INCLUDE_DIRS} lib: ${VORBIS_LIBRARIES}")
else()
    MESSAGE(STATUS "Could NOT find Vorbis.")
endif()