#ifndef OPENGL_H
#define OPENGL_H

#if defined(RE_PLATFORM_MAC)
    #include <GL/glew.h>
    #ifdef GL_ES_VERSION_2_0
        #include <OpenGLES/ES2/gl.h>
    #else
        #include <OpenGL/gl.h>
    #endif

    #define GL_HALF_FLOAT_OES GL_FLOAT
    #define GL_ETC1_RGB8_OES 0

    #define GLES 0
#elif defined(RE_PLATFORM_WIN)
    #include <Windows.h>
    #include <GL/glew.h>
    #include <GL/wglew.h>

    #define GLES 0
#elif defined(RE_PLATFORM_LINUX)
    #include <GL/glew.h>
    #include <GL/gl.h>
    #include <GL/glext.h>

    #define GL_HALF_FLOAT_OES GL_FLOAT
    #define GL_ETC1_RGB8_OES 0

    #define GLES 0
#elif defined (RE_PLATFORM_ANDROID)
    #define GLES 1

    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>

    #define GL_SAMPLER_1D 0x8B5D
    #define GL_SAMPLER_3D 0x8B5F
    #define GL_SAMPLER_2D_RECT_SHADOW 0x8B64

    #define GL_TEXTURE_1D 0x0DE0
    #define GL_TEXTURE_3D 0x806F
    #define GL_DEPTH_COMPONENT24 0x81A6
    #define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
    #define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

    #define GL_RGBA16 0x805B
    #define GL_RED 0x1903
    #define GL_RG 0x8227

    // TODO: No define in gles2
    #define GL_DEPTH_STENCIL 1
    #define GL_DEPTH_STENCIL_ATTACHMENT 1
#elif defined (RE_PLATFORM_IOS)
    #define GLES 1

    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>

    #define GL_SAMPLER_1D 0x8B5D
    #define GL_SAMPLER_3D 0x8B5F
    #define GL_SAMPLER_2D_RECT_SHADOW 0x8B64

    #define GL_TEXTURE_1D 0x0DE0
    #define GL_TEXTURE_3D 0x806F
    #define GL_DEPTH_COMPONENT24 0x81A6
    #define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
    #define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
    #define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

    #define GL_RGBA16 0x805B
    #define GL_RED 0x1903
    #define GL_RG 0x8227

    // TODO: No define in gles2
    #define GL_DEPTH_STENCIL 1
    #define GL_DEPTH_STENCIL_ATTACHMENT 1

    #define GL_ETC1_RGB8_OES 0

#else
    #include "CCGL.h"

    #define GL_SAMPLER_1D 0x8B5D
    #define GL_SAMPLER_3D 0x8B5F
    #define GL_SAMPLER_2D_RECT_SHADOW 0x8B64

    #define GL_RGBA16 0x805B
#endif

#endif // OPENGL_H
