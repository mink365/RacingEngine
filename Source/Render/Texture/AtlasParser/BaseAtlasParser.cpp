/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "BaseAtlasParser.h"
#include "Texture/Frame/TextureFrame.h"
#include "Texture/Frame/TextureFrameManager.h"
#include "Texture/TextureManager.h"

namespace re {

BaseAtlasParser::BaseAtlasParser()
{
}

BaseAtlasParser::~BaseAtlasParser()
{

}

std::vector<TextureFramePtr> BaseAtlasParser::getTextureFrames()
{
    std::vector<TextureFramePtr> list;

    for (auto& data : this->frameDatas) {
        auto frame = this->getTextureFrameFromData(data);

        list.push_back(frame);
    }

    return list;
}

TextureFrameData &BaseAtlasParser::allocateFrameData()
{
    TextureFrameData data;
    this->frameDatas.push_back(data);

    return this->frameDatas.back();
}

void BaseAtlasParser::getTextureByImageFile()
{
    const std::string& path = this->imageFile->getName();
    int pos2 = path.find_last_of(".");
    auto name = path.substr(0, pos2);

    if (TextureManager::getInstance().containTexture(name)) {
        texture = TextureManager::getInstance().getTexture(name);
    } else {
        texture = Texture::create();
        texture->setFile(this->imageFile);

        TextureManager::getInstance().registerTexture(texture);
    }
}

TextureFramePtr BaseAtlasParser::getTextureFrameFromData(const TextureFrameData &data)
{
    TextureFrame::ptr frame = TextureFrame::create();
    frame->setTexture(texture);

    frame->name = data.name;
    frame->isRotated = data.isRotated;
    frame->region = data.region;

    frame->isTrimmed = data.isTrimmed;
    frame->offset = data.offset;
    frame->originalSize = data.originalSize;

    TextureFrameManager::getInstance().registerFrame(frame);
}

string GetFrameName(const FilePtr& atlasFile, const string &name)
{
    auto dir = atlasFile->getDirPath();

    // TODO: use the path
    return name;
}

FilePtr GetImageFile(const FilePtr& atlasFile, const string &imageName)
{
    auto dir = atlasFile->getDirPath();

    return FileSystem::getInstance().openFile(dir+"/"+imageName, fsMode::Read);
}

}
