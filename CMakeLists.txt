cmake_minimum_required(VERSION 3.4)
project(RacingEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

SET(RE_MAJOR_VERSION 0)
SET(RE_MINOR_VERSION 1)
SET(RE_PATCH_VERSION 0)
SET(RE_VERSION ${RE_MAJOR_VERSION}.${RE_MINOR_VERSION}.${RE_PATCH_VERSION})

find_path(RE_PROJECT_DIR NAMES README.md
    PATHS 
    ${CMAKE_SOURCE_DIR}
    NO_DEFAULT_PATH
    )

MARK_AS_ADVANCED(RE_PROJECT_DIR)
SET(RE_EXAMPLES_DIR "${RE_PROJECT_DIR}/Examples")
SET(RE_EXAMPLES_SOURCE_DIR "${RE_EXAMPLES_DIR}/Source")
SET(RE_SOURCE_DIR "${RE_PROJECT_DIR}/Source")
SET(RE_EXTERNAL_DIR "${RE_PROJECT_DIR}/Source/External")

if(ANDROID)
    set(RE_PLATFORM_ANDROID 1)
    set(RE_RESOURCES_SEARCH_PATH "asset:")

    add_definitions(-DRE_PLATFORM_ANDROID -DFMT_HEADER_ONLY -DRE_RESOURCES_SEARCH_PATH="${RE_RESOURCES_SEARCH_PATH}")
elseif(IOS)
    set(RE_PLATFORM_IOS 1)

    add_definitions(-DRE_PLATFORM_IOS -DFMT_HEADER_ONLY)
else(APPLE)
    set(RE_PLATFORM_MAC 1)
    set(RE_RESOURCES_SEARCH_PATH "${RE_EXAMPLES_DIR}/Resources/")

    add_definitions(-DRE_PLATFORM_MAC -DFMT_HEADER_ONLY -DRE_RESOURCES_SEARCH_PATH="${RE_RESOURCES_SEARCH_PATH}")
endif()

set(BASE_INCLUDE_DIRECTORIES
        ${RE_SOURCE_DIR}
        ${RE_SOURCE_DIR}/Platform
        ${RE_SOURCE_DIR}/Render
        ${RE_SOURCE_DIR}/Scene
        ${RE_SOURCE_DIR}/Resource
        ${RE_SOURCE_DIR}/External
        ${RE_SOURCE_DIR}/External/hana/include/
        ${RE_SOURCE_DIR}/External/universal-tween-engine-cpp/
        ${RE_SOURCE_DIR}/External/yaml-cpp/include/
        )

# path need to be absolute
list(INSERT CMAKE_MODULE_PATH 0 "${RE_PROJECT_DIR}/Build/CMake/Modules/")

find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(OpenAL REQUIRED)
find_package(Vorbis REQUIRED)
find_package(Bullet REQUIRED)

set(RE_INCLUDE_DIRECTORIES
        ${FREETYPE_INCLUDE_DIRS}
        ${PNG_INCLUDE_DIRS}
        ${JPEG_INCLUDE_DIRS}
        ${OPENAL_INCLUDE_DIRS}
        ${VORBIS_INCLUDE_DIRS}
        ${BULLET_INCLUDE_DIRS}
        )

set(RE_LINK_LIBRARIES
        ${FREETYPE_LIBRARIES}
        ${PNG_LIBRARIES}
        ${JPEG_LIBRARIES}
        ${OPENAL_LIBRARIES}
        ${VORBIS_LIBRARIES}
        ${BULLET_LIBRARIES}
        )

if(ANDROID)
    include(AndroidNdkModules REQUIRED)
    include(android.apk REQUIRED)
    android_ndk_import_module_ndk_helper()

    find_package(ZLib REQUIRED)

    list(APPEND RE_INCLUDE_DIRECTORIES
            ${ZLIB_INCLUDE_DIRS}

            ${RE_SOURCE_DIR}/Platform/android
            )

    list(APPEND RE_LINK_LIBRARIES 
            ${ZLIB_LIBRARIES}
            ndk_helper
            OpenSLES
            )
elseif(IOS)
    list(APPEND RE_INCLUDE_DIRECTORIES
            ${RE_SOURCE_DIR}/Platform/iOS
            )

    ADD_FRAMEWORK(OpenGLES RE_LINK_LIBRARIES)
    ADD_FRAMEWORK(UIKit RE_LINK_LIBRARIES)
    ADD_FRAMEWORK(QuartzCore RE_LINK_LIBRARIES)

    # add libz.dylib needed by png
    list(APPEND RE_LINK_LIBRARIES "z")

elseif(${RE_PLATFORM_MAC})
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(GLFW REQUIRED)

    list(APPEND RE_INCLUDE_DIRECTORIES
            ${OPENGL_INCLUDE_DIRS}
            ${GLEW_INCLUDE_DIRS}
            ${GLFW_INCLUDE_DIRS}

            ${RE_SOURCE_DIR}/Platform/linux
            )

    list(APPEND RE_LINK_LIBRARIES
            ${OPENGL_LIBRARIES}
            ${GLEW_LIBRARIES}
            ${GLFW_LIBRARIES}
            )
endif()

include_directories(
        ${BASE_INCLUDE_DIRECTORIES}
        ${RE_INCLUDE_DIRECTORIES}
)

# Projects
ADD_SUBDIRECTORY(Build/CMake/External/Tween)
ADD_SUBDIRECTORY(Source/External/yaml-cpp EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(Build/CMake/Core)
ADD_SUBDIRECTORY(Build/CMake/Examples)
