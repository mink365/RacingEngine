#ifndef OPENGL_H
#define OPENGL_H

#if defined(__APPLE__)
#   include <GL/glew.h>
#  ifdef GL_ES_VERSION_2_0
#    include <OpenGLES/ES2/gl.h>
#  else
#    include <OpenGL/gl.h>
#  endif
#elif defined(_WIN32) || defined(_WIN64)
#  include <Windows.h>
#  include <GL/glew.h>
#  include <GL/wglew.h>
#elif defined(_LINUX)
#  include <GL/glew.h>
#  include <GL/gl.h>
#else
    #include "CCGL.h"

    #define GL_SAMPLER_1D 0x8B5D
    #define GL_SAMPLER_3D 0x8B5F
    #define GL_SAMPLER_2D_RECT_SHADOW 0x8B64

    #define GL_RGBA16 0x805B
#endif

#endif // OPENGL_H
