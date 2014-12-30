#include "TextureParser.h"

#include "FileSystem/FileSystem.h"
#include "Texture/TextureManager.h"
#include "Texture/Frame/TextureFrameManager.h"
#include "Resource/AtlasParser/BaseAtlasParser.h"
#include "Resource/AtlasParser/JSONAtlasParser.h"
#include <iostream>

namespace re {

TextureParser::TextureParser()
{
    factory.registerCreateFunc("json", [](){
        return std::make_shared<JSONAtlasParser>();
    });
}

void TextureParser::addTextures(const string &dir, const std::string &extension)
{
    auto list = FileSystem::getInstance().listFilesTree(dir, extension);

    for (FilePtr& file : list) {
        auto ext = file->getExt();
        if (ext == "json") {
            auto parser = factory.createInstance(ext);
            parser->parseAtlasFile(file);

            parser->getTextureFrames();
        } else {
            // if just a image
            this->parseSingle(file);
        }
    }
}

void TextureParser::parseSingle(FilePtr &file)
{
    Texture::ptr texture = Texture::create();
    texture->setFile(file);

    TextureManager::getInstance().registerTexture(texture);
    LOG_D("register texture: %s", texture->getName().c_str());
    // TODO: get the size of tex
    TextureManager::getInstance().loadTextures();

    TextureFrame::ptr frame = TextureFrame::create();
    frame->setTexture(texture);
    frame->init(file->getName(), false, 0, 0, texture->getWidth(), texture->getHeight());

    TextureFrameManager::getInstance().registerFrame(frame);
}

} // namespace re
