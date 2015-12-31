# - Try to find GLFW
# Once done this will define
# GLFW_INCLUDE_DIRS, where to find glfw3.h, etc.
# GLFW_LIBRARIES, the libraries to link against to use GLFW.
# GLFW_FOUND, If false, do not try to use GLFW.

find_path(GLFW_INCLUDE_DIRS
		NAMES GLFW/glfw3.h
        )

find_library(GLFW_LIBRARIES
        NAMES glfw3
        )

if(GLFW_INCLUDE_DIRS)
    set(GLFW_FOUND TRUE)
    MESSAGE(STATUS "Found GLFW's include: ${GLFW_INCLUDE_DIRS} lib: ${GLFW_LIBRARIES}")
else()
    MESSAGE(STATUS "Could NOT find GLFW.")
endif()