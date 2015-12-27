cmake_minimum_required(VERSION 3.3)
project(RacingEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# set(SOURCE_FILES main.cpp base_func.cpp base_func.h)
# add_executable(RacingEngine ${SOURCE_FILES})

# Projects
ADD_SUBDIRECTORY(Core)
ADD_SUBDIRECTORY(Examples)

# ADD_SUBDIRECTORY(Plugins/Scene/OCTree)
# ADD_SUBDIRECTORY(Plugins/Input/MsgInput)
# ADD_SUBDIRECTORY(Plugins/Script/Python)