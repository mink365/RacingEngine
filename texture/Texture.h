#ifndef TEXTURE_H
#define TEXTURE_H

#include "platform.h"
#include <string>

class Texture
{
public:
    Texture();
    Texture(const int width, const int height, const int glID);

    GPU_ID getGlID() const;
    void setGlID(GPU_ID value);

    int getHeight() const;

    int getWidth() const;

    std::string getPath() const;
    void setPath(const std::string &value);

    std::string getName() const;
private:
    std::string path;
    std::string name;

    int width;
    int height;
    GPU_ID glID;
};

#endif // TEXTURE_H
