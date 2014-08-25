#ifndef OPENGL_H
#define OPENGL_H

#if defined(RE_PLATFORM_MAC)
    #include <GL/glew.h>
    #ifdef GL_ES_VERSION_2_0
        #include <OpenGLES/ES2/gl.h>
    #else
        #include <OpenGL/gl.h>
    #endif
#elif defined(RE_PLATFORM_WIN)
    #include <Windows.h>
    #include <GL/glew.h>
    #include <GL/wglew.h>
#elif defined(RE_PLATFORM_LINUX)
    #include <GL/glew.h>
    #include <GL/gl.h>
    #include <GL/glext.h>

    #define GL_HALF_FLOAT_OES GL_FLOAT
    #define GL_ETC1_RGB8_OES 0
#elif defined (RE_PLATFORM_ANDROID)
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>

    #define GL_SAMPLER_1D 0x8B5D
    #define GL_SAMPLER_3D 0x8B5F
    #define GL_SAMPLER_2D_RECT_SHADOW 0x8B64

    #define GL_RGBA16 0x805B
    #define GL_RED 0x1903
    #define GL_RG 0x8227

    // TODO: No define in gles2
    #define GL_DEPTH_STENCIL 1
    #define GL_DEPTH_STENCIL_ATTACHMENT 1
#else
    #include "CCGL.h"

    #define GL_SAMPLER_1D 0x8B5D
    #define GL_SAMPLER_3D 0x8B5F
    #define GL_SAMPLER_2D_RECT_SHADOW 0x8B64

    #define GL_RGBA16 0x805B
#endif

#endif // OPENGL_H
