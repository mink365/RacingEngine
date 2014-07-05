#include "TextureParser.h"

#include "FileSystem/FileSystem.h"
#include "Texture/TextureManager.h"
#include "Texture/Frame/TextureFrameManager.h"

namespace re {

TextureParser::TextureParser()
{
}

void TextureParser::addTextures(const string &dir)
{
    auto list = FileSystem::getInstance().listFilesTree(dir);

    for (std::shared_ptr<const File> file : list) {
        this->parseSingle(file);
    }
}

void TextureParser::parseSingle(ConstFilePtr &file)
{
    Texture::ptr texture = Texture::create();
    texture->setFile(file);

    TextureFrame::ptr frame = TextureFrame::create();
    frame->setTexture(texture);
    frame->init(file->getFullPath(), 0, 0, 0, texture->getWidth(), texture->getHeight());

    TextureManager::getInstance().registerTexture(texture);
    TextureFrameManager::getInstance().registerFrame(frame);
}

} // namespace re
