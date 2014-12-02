/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef BASEATLASPARSER_H
#define BASEATLASPARSER_H

#include "Math/BoundingVolume.h"
#include "FileSystem/FileSystem.h"

namespace re {

struct TextureFrameData {
    std::string name;

    bool isTrimmed;
    bool isRotated;

    Rect region;

    Size originalSize;
    Vec2 offset;
};

std::string GetFrameName(const FilePtr &atlasFile, const std::string& name);
FilePtr GetImageFile(const FilePtr &atlasFile, const std::string& imageName);

class BaseAtlasParser
{
public:
    BaseAtlasParser();
    virtual ~BaseAtlasParser();

    std::vector<TextureFramePtr> getTextureFrames();

    virtual void parseAtlasFile(FilePtr& file) = 0;

protected:
    TextureFrameData& allocateFrameData();

    void getTextureByImageFile();

private:
    TextureFramePtr getTextureFrameFromData(const TextureFrameData& data);

protected:
    std::vector<TextureFrameData> frameDatas;

    FilePtr atlasFile;

    Size imageSize;
    FilePtr imageFile;
    TexturePtr texture;
};

}

#endif // BASEATLASPARSER_H
