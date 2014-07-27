#include "GLCheckUtil.h"

#include "opengl.h"

#include <sstream>
#include <iostream>

GLCheckUtil::GLCheckUtil()
{
}

void DrawCube(GLuint textureId, int size)
{
    glBindTexture(GL_TEXTURE_2D, textureId);

    glColor4f(1, 1, 1, 1);
    glBegin(GL_TRIANGLES);
        // front faces
        glNormal3f(0,0,1);
        // face v0-v1-v2
        glTexCoord2f(1,1);  glVertex3f(size,size,size);
        glTexCoord2f(0,1);  glVertex3f(-size,size,size);
        glTexCoord2f(0,0);  glVertex3f(-size,-size,size);
        // face v2-v3-v0
        glTexCoord2f(0,0);  glVertex3f(-size,-size,size);
        glTexCoord2f(1,0);  glVertex3f(size,-size,size);
        glTexCoord2f(1,1);  glVertex3f(size,size,size);

        // right faces
        glNormal3f(1,0,0);
        // face v0-v3-v4
        glTexCoord2f(0,1);  glVertex3f(size,size,size);
        glTexCoord2f(0,0);  glVertex3f(size,-size,size);
        glTexCoord2f(1,0);  glVertex3f(size,-size,-size);
        // face v4-v5-v0
        glTexCoord2f(1,0);  glVertex3f(size,-size,-size);
        glTexCoord2f(1,1);  glVertex3f(size,size,-size);
        glTexCoord2f(0,1);  glVertex3f(size,size,size);

        // top faces
        glNormal3f(0,1,0);
        // face v0-v5-v6
        glTexCoord2f(1,0);  glVertex3f(size,size,size);
        glTexCoord2f(1,1);  glVertex3f(size,size,-size);
        glTexCoord2f(0,1);  glVertex3f(-size,size,-size);
        // face v6-v1-v0
        glTexCoord2f(0,1);  glVertex3f(-size, size, -size);
        glTexCoord2f(0,0);  glVertex3f(-size, size, size);
        glTexCoord2f(1,0);  glVertex3f(size, size, size);

        // left faces
        glNormal3f(-1,0,0);
        // face  v1-v6-v7
        glTexCoord2f(1,1);  glVertex3f(-size, size, size);
        glTexCoord2f(0,1);  glVertex3f(-size, size, -size);
        glTexCoord2f(0,0);  glVertex3f(-size, -size, -size);
        // face v7-v2-v1
        glTexCoord2f(0,0);  glVertex3f(-size, -size, -size);
        glTexCoord2f(1,0);  glVertex3f(-size, -size, size);
        glTexCoord2f(1,1);  glVertex3f(-size, size, size);

        // bottom faces
        glNormal3f(0,-1,0);
        // face v7-v4-v3
        glTexCoord2f(0,0);  glVertex3f(-size, -size, -size);
        glTexCoord2f(1,0);  glVertex3f(size, -size, -size);
        glTexCoord2f(1,1);  glVertex3f(size, -size, size);
        // face v3-v2-v7
        glTexCoord2f(1,1);  glVertex3f(size, -size, size);
        glTexCoord2f(0,1);  glVertex3f(-size, -size, size);
        glTexCoord2f(0,0);  glVertex3f(-size, -size, -size);

        // back faces
        glNormal3f(0,0,-1);
        // face v4-v7-v6
        glTexCoord2f(0,0);  glVertex3f(size, -size, -size);
        glTexCoord2f(1,0);  glVertex3f(-size, -size, -size);
        glTexCoord2f(1,1);  glVertex3f(-size, size, -size);
        // face v6-v5-v4
        glTexCoord2f(1,1);  glVertex3f(-size, size, -size);
        glTexCoord2f(0,1);  glVertex3f(size, size, -size);
        glTexCoord2f(0,0);  glVertex3f(size, -size, -size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

///////////////////////////////////////////////////////////////////////////////
// check FBO completeness
///////////////////////////////////////////////////////////////////////////////
bool checkFramebufferStatus()
{
    // check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        std::cout << "Framebuffer complete." << std::endl;
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
        return false;
/*
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
        std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
        std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
        return false;
*/
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        std::cout << "[ERROR] Framebuffer incomplete: Unsupported by FBO implementation." << std::endl;
        return false;

    default:
        std::cout << "[ERROR] Framebuffer incomplete: Unknown error." << std::endl;
        return false;
    }
}



///////////////////////////////////////////////////////////////////////////////
// print out the FBO infos
///////////////////////////////////////////////////////////////////////////////
void printFramebufferInfo()
{
    std::cout << "\n===== FBO STATUS =====\n";

    // print max # of colorbuffers supported by FBO
    int colorBufferCount = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
    std::cout << "Max Number of Color Buffer Attachment Points: " << colorBufferCount << std::endl;

    int objectType;
    int objectId;

    // print info of the colorbuffer attachable image
    for(int i = 0; i < colorBufferCount; ++i)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_COLOR_ATTACHMENT0+i,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                              &objectType);
        if(objectType != GL_NONE)
        {
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                                  GL_COLOR_ATTACHMENT0+i,
                                                  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                                  &objectId);

            std::string formatName;

            std::cout << "Color Attachment " << i << ": ";
            if(objectType == GL_TEXTURE)
            {
                std::cout << "GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
            }
            else if(objectType == GL_RENDERBUFFER)
            {
                std::cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
            } else {
                std::cout << "no avalive type" << std::endl;
            }


        }
    }

    // print info of the depthbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_DEPTH_ATTACHMENT,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &objectType);
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_DEPTH_ATTACHMENT,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                              &objectId);

        std::cout << "Depth Attachment: ";
        switch(objectType)
        {
        case GL_TEXTURE:
            std::cout << "GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
            break;
        case GL_RENDERBUFFER:
            std::cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
            break;
        }
    }

    // print info of the stencilbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_STENCIL_ATTACHMENT,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &objectType);
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_STENCIL_ATTACHMENT,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                              &objectId);

        std::cout << "Stencil Attachment: ";
        switch(objectType)
        {
        case GL_TEXTURE:
            std::cout << "GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
            break;
        case GL_RENDERBUFFER:
            std::cout << "GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
            break;
        }
    }

    std::cout << std::endl;
}



///////////////////////////////////////////////////////////////////////////////
// return texture parameters as string using glGetTexLevelParameteriv()
///////////////////////////////////////////////////////////////////////////////
std::string getTextureParameters(GLuint id)
{
    if(glIsTexture(id) == GL_FALSE)
        return "Not texture object";

    int width, height, format;
    std::string formatName;
    glBindTexture(GL_TEXTURE_2D, id);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);            // get texture width
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);          // get texture height
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format); // get texture internal format
    glBindTexture(GL_TEXTURE_2D, 0);

    formatName = convertInternalFormatToString(format);

    std::stringstream ss;
    ss << width << "x" << height << ", " << formatName;
    return ss.str();
}



///////////////////////////////////////////////////////////////////////////////
// return renderbuffer parameters as string using glGetRenderbufferParameteriv
///////////////////////////////////////////////////////////////////////////////
std::string getRenderbufferParameters(GLuint id)
{
    if(glIsRenderbuffer(id) == GL_FALSE)
        return "Not Renderbuffer object";

    int width, height, format;
    std::string formatName;
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);    // get renderbuffer width
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);  // get renderbuffer height
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &format); // get renderbuffer internal format
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    formatName = convertInternalFormatToString(format);

    std::stringstream ss;
    ss << width << "x" << height << ", " << formatName;
    return ss.str();
}



///////////////////////////////////////////////////////////////////////////////
// convert OpenGL internal format enum to string
///////////////////////////////////////////////////////////////////////////////
std::string convertInternalFormatToString(GLenum format)
{
    std::string formatName;

    switch(format)
    {
    case GL_STENCIL_INDEX:      // 0x1901
        formatName = "GL_STENCIL_INDEX";
        break;
    case GL_DEPTH_COMPONENT:    // 0x1902
        formatName = "GL_DEPTH_COMPONENT";
        break;
    case GL_ALPHA:              // 0x1906
        formatName = "GL_ALPHA";
        break;
    case GL_RGB:                // 0x1907
        formatName = "GL_RGB";
        break;
    case GL_RGBA:               // 0x1908
        formatName = "GL_RGBA";
        break;
    case GL_LUMINANCE:          // 0x1909
        formatName = "GL_LUMINANCE";
        break;
    case GL_LUMINANCE_ALPHA:    // 0x190A
        formatName = "GL_LUMINANCE_ALPHA";
        break;
    case GL_R3_G3_B2:           // 0x2A10
        formatName = "GL_R3_G3_B2";
        break;
    case GL_ALPHA4:             // 0x803B
        formatName = "GL_ALPHA4";
        break;
    case GL_ALPHA8:             // 0x803C
        formatName = "GL_ALPHA8";
        break;
    case GL_ALPHA12:            // 0x803D
        formatName = "GL_ALPHA12";
        break;
    case GL_ALPHA16:            // 0x803E
        formatName = "GL_ALPHA16";
        break;
    case GL_LUMINANCE4:         // 0x803F
        formatName = "GL_LUMINANCE4";
        break;
    case GL_LUMINANCE8:         // 0x8040
        formatName = "GL_LUMINANCE8";
        break;
    case GL_LUMINANCE12:        // 0x8041
        formatName = "GL_LUMINANCE12";
        break;
    case GL_LUMINANCE16:        // 0x8042
        formatName = "GL_LUMINANCE16";
        break;
    case GL_LUMINANCE4_ALPHA4:  // 0x8043
        formatName = "GL_LUMINANCE4_ALPHA4";
        break;
    case GL_LUMINANCE6_ALPHA2:  // 0x8044
        formatName = "GL_LUMINANCE6_ALPHA2";
        break;
    case GL_LUMINANCE8_ALPHA8:  // 0x8045
        formatName = "GL_LUMINANCE8_ALPHA8";
        break;
    case GL_LUMINANCE12_ALPHA4: // 0x8046
        formatName = "GL_LUMINANCE12_ALPHA4";
        break;
    case GL_LUMINANCE12_ALPHA12:// 0x8047
        formatName = "GL_LUMINANCE12_ALPHA12";
        break;
    case GL_LUMINANCE16_ALPHA16:// 0x8048
        formatName = "GL_LUMINANCE16_ALPHA16";
        break;
    case GL_INTENSITY:          // 0x8049
        formatName = "GL_INTENSITY";
        break;
    case GL_INTENSITY4:         // 0x804A
        formatName = "GL_INTENSITY4";
        break;
    case GL_INTENSITY8:         // 0x804B
        formatName = "GL_INTENSITY8";
        break;
    case GL_INTENSITY12:        // 0x804C
        formatName = "GL_INTENSITY12";
        break;
    case GL_INTENSITY16:        // 0x804D
        formatName = "GL_INTENSITY16";
        break;
    case GL_RGB4:               // 0x804F
        formatName = "GL_RGB4";
        break;
    case GL_RGB5:               // 0x8050
        formatName = "GL_RGB5";
        break;
    case GL_RGB8:               // 0x8051
        formatName = "GL_RGB8";
        break;
    case GL_RGB10:              // 0x8052
        formatName = "GL_RGB10";
        break;
    case GL_RGB12:              // 0x8053
        formatName = "GL_RGB12";
        break;
    case GL_RGB16:              // 0x8054
        formatName = "GL_RGB16";
        break;
    case GL_RGBA2:              // 0x8055
        formatName = "GL_RGBA2";
        break;
    case GL_RGBA4:              // 0x8056
        formatName = "GL_RGBA4";
        break;
    case GL_RGB5_A1:            // 0x8057
        formatName = "GL_RGB5_A1";
        break;
    case GL_RGBA8:              // 0x8058
        formatName = "GL_RGBA8";
        break;
    case GL_RGB10_A2:           // 0x8059
        formatName = "GL_RGB10_A2";
        break;
    case GL_RGBA12:             // 0x805A
        formatName = "GL_RGBA12";
        break;
    case GL_RGBA16:             // 0x805B
        formatName = "GL_RGBA16";
        break;
    case GL_DEPTH_COMPONENT16:  // 0x81A5
        formatName = "GL_DEPTH_COMPONENT16";
        break;
    case GL_DEPTH_COMPONENT24:  // 0x81A6
        formatName = "GL_DEPTH_COMPONENT24";
        break;
    case GL_DEPTH_COMPONENT32:  // 0x81A7
        formatName = "GL_DEPTH_COMPONENT32";
        break;
    case GL_DEPTH_STENCIL:      // 0x84F9
        formatName = "GL_DEPTH_STENCIL";
        break;
    case GL_RGBA32F:            // 0x8814
        formatName = "GL_RGBA32F";
        break;
    case GL_RGB32F:             // 0x8815
        formatName = "GL_RGB32F";
        break;
    case GL_RGBA16F:            // 0x881A
        formatName = "GL_RGBA16F";
        break;
    case GL_RGB16F:             // 0x881B
        formatName = "GL_RGB16F";
        break;
    case GL_DEPTH24_STENCIL8:   // 0x88F0
        formatName = "GL_DEPTH24_STENCIL8";
        break;
    default:
        std::stringstream ss;
        ss << "Unknown Format(0x" << std::hex << format << ")" << std::ends;
        formatName = ss.str();
    }

    return formatName;
}


void DrawCube(GLuint textureId)
{
    glBindTexture(GL_TEXTURE_2D, textureId);

    glColor4f(1, 1, 1, 1);
    glBegin(GL_TRIANGLES);
        // front faces
        glNormal3f(0,0,1);
        // face v0-v1-v2
        glTexCoord2f(1,1);  glVertex3f(1,1,1);
        glTexCoord2f(0,1);  glVertex3f(-1,1,1);
        glTexCoord2f(0,0);  glVertex3f(-1,-1,1);
        // face v2-v3-v0
        glTexCoord2f(0,0);  glVertex3f(-1,-1,1);
        glTexCoord2f(1,0);  glVertex3f(1,-1,1);
        glTexCoord2f(1,1);  glVertex3f(1,1,1);

        // right faces
        glNormal3f(1,0,0);
        // face v0-v3-v4
        glTexCoord2f(0,1);  glVertex3f(1,1,1);
        glTexCoord2f(0,0);  glVertex3f(1,-1,1);
        glTexCoord2f(1,0);  glVertex3f(1,-1,-1);
        // face v4-v5-v0
        glTexCoord2f(1,0);  glVertex3f(1,-1,-1);
        glTexCoord2f(1,1);  glVertex3f(1,1,-1);
        glTexCoord2f(0,1);  glVertex3f(1,1,1);

        // top faces
        glNormal3f(0,1,0);
        // face v0-v5-v6
        glTexCoord2f(1,0);  glVertex3f(1,1,1);
        glTexCoord2f(1,1);  glVertex3f(1,1,-1);
        glTexCoord2f(0,1);  glVertex3f(-1,1,-1);
        // face v6-v1-v0
        glTexCoord2f(0,1);  glVertex3f(-1,1,-1);
        glTexCoord2f(0,0);  glVertex3f(-1,1,1);
        glTexCoord2f(1,0);  glVertex3f(1,1,1);

        // left faces
        glNormal3f(-1,0,0);
        // face  v1-v6-v7
        glTexCoord2f(1,1);  glVertex3f(-1,1,1);
        glTexCoord2f(0,1);  glVertex3f(-1,1,-1);
        glTexCoord2f(0,0);  glVertex3f(-1,-1,-1);
        // face v7-v2-v1
        glTexCoord2f(0,0);  glVertex3f(-1,-1,-1);
        glTexCoord2f(1,0);  glVertex3f(-1,-1,1);
        glTexCoord2f(1,1);  glVertex3f(-1,1,1);

        // bottom faces
        glNormal3f(0,-1,0);
        // face v7-v4-v3
        glTexCoord2f(0,0);  glVertex3f(-1,-1,-1);
        glTexCoord2f(1,0);  glVertex3f(1,-1,-1);
        glTexCoord2f(1,1);  glVertex3f(1,-1,1);
        // face v3-v2-v7
        glTexCoord2f(1,1);  glVertex3f(1,-1,1);
        glTexCoord2f(0,1);  glVertex3f(-1,-1,1);
        glTexCoord2f(0,0);  glVertex3f(-1,-1,-1);

        // back faces
        glNormal3f(0,0,-1);
        // face v4-v7-v6
        glTexCoord2f(0,0);  glVertex3f(1,-1,-1);
        glTexCoord2f(1,0);  glVertex3f(-1,-1,-1);
        glTexCoord2f(1,1);  glVertex3f(-1,1,-1);
        // face v6-v5-v4
        glTexCoord2f(1,1);  glVertex3f(-1,1,-1);
        glTexCoord2f(0,1);  glVertex3f(1,1,-1);
        glTexCoord2f(0,0);  glVertex3f(1,-1,-1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}
