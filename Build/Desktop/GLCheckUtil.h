#ifndef GLCHECKUTIL_H
#define GLCHECKUTIL_H

#include "opengl.h"
#include <string>

class GLCheckUtil
{
public:
    GLCheckUtil();
};

void DrawCube(GLuint textureId);
void DrawCube(GLuint textureId, int size);

std::string convertInternalFormatToString(GLenum format);
std::string getRenderbufferParameters(GLuint id);
std::string getTextureParameters(GLuint id);
void printFramebufferInfo();
bool checkFramebufferStatus();

#endif // GLCHECKUTIL_H
