#include "TextureParser.h"

#include "FileSystem/FileSystem.h"
#include "Texture/TextureManager.h"
#include "Texture/Frame/TextureFrame.h"
#include "Resource/AtlasParser/BaseAtlasParser.h"
#include "Resource/AtlasParser/JSONAtlasParser.h"
#include <iostream>

namespace re {

TextureParser::TextureParser()
{
    factory.registerCreateFunc("json", [](){
        return Create<JSONAtlasParser>();
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

    TextureManager::instance().registerTexture(texture);

    // TODO: get the size of tex
    TextureManager::instance().loadTextures();

    TextureFrame::ptr frame = TextureFrame::create();
    frame->setTexture(texture);
    frame->init(file->getName(), false, 0, 0, texture->getWidth(), texture->getHeight());

    TextureFrameManager::instance().Register(frame);
}

} // namespace re
