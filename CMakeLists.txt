cmake_minimum_required(VERSION 3.4)
project(RacingEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

SET(RE_MAJOR_VERSION 0)
SET(RE_MINOR_VERSION 1)
SET(RE_PATCH_VERSION 0)
SET(RE_VERSION ${RE_MAJOR_VERSION}.${RE_MINOR_VERSION}.${RE_PATCH_VERSION})

FIND_PATH(RE_PROJECT_DIR racing.pro PATHS .)

MARK_AS_ADVANCED(RE_PROJECT_DIR)
SET(RE_EXAMPLES_DIR "${RE_PROJECT_DIR}/Examples/Source")
SET(RE_SOURCE_DIR "${RE_PROJECT_DIR}/Source")

set(RE_PLATFORM_MAC 1)

add_definitions(-DRE_PLATFORM_MAC -DFMT_HEADER_ONLY)

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
        /usr/local/include/
        /usr/local/include/freetype2/
        /usr/local/include/bullet/
        /usr/local/opt/openal-soft/include
        )

if(${RE_PLATFORM_MAC})
    include_directories(
            ${BASE_INCLUDE_DIRECTORIES}
            ${RE_SOURCE_DIR}/Platform/linux
    )
endif()

link_directories(
        /usr/local/lib
        /usr/local/opt/openal-soft/lib
        )
SET(CMAKE_EXE_LINKER_FLAGS "-v -lglfw3 -lopenal -lvorbis -lvorbisenc -lvorbisfile -logg")

# Projects
ADD_SUBDIRECTORY(Build/CMake/Core)
ADD_SUBDIRECTORY(Build/CMake/Examples)



#add_executable(RacingEngine ${SOURCE_FILES})