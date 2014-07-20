#include "TextureParser.h"

#include "FileSystem/FileSystem.h"
#include "Texture/TextureManager.h"
#include "Texture/Frame/TextureFrameManager.h"
#include <iostream>

namespace re {

TextureParser::TextureParser()
{
}

void TextureParser::addTextures(const string &dir, const std::string &extension)
{
    auto list = FileSystem::getInstance().listFilesTree(dir, extension);

    for (std::shared_ptr<File> file : list) {
        this->parseSingle(file);
    }
}

void TextureParser::parseSingle(FilePtr &file)
{
    Texture::ptr texture = Texture::create();
    texture->setFile(file);

    TextureManager::getInstance().registerTexture(texture);
    std::cout << "register texture: " << texture->getName() << std::endl;
    // TODO: get the size of tex
    TextureManager::getInstance().loadTextures();

    TextureFrame::ptr frame = TextureFrame::create();
    frame->setTexture(texture);
    frame->init(file->getName(), false, 0, 0, texture->getWidth(), texture->getHeight());

    TextureFrameManager::getInstance().registerFrame(frame);
}

} // namespace re
