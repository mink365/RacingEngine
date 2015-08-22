/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "AngleFontParser.h"

#include "tinyxml2/tinyxml2.h"

#include "FileSystem/File.h"
#include "Font/Glyph.h"
#include "Font/Font.h"

#include "Texture/TextureManager.h"

using namespace tinyxml2;

namespace re {

AngleFontParser::AngleFontParser()
{
}

void AngleFontParser::parseFile(FilePtr& file)
{
    XMLDocument doc;

    ByteBufferPtr buf = file->read();
    std::string data((char*)(buf->getData()), buf->getSize());

    doc.Parse(data.c_str());

    XMLElement* root = doc.RootElement();

    XMLElement* info = root->FirstChildElement("info");
    this->size = info->IntAttribute("size");
    this->family = info->Attribute("face");
//    this->style = info->Attribute("style");

    XMLElement* common = root->FirstChildElement("common");
    this->width = common->IntAttribute("scaleW");
    this->height = common->IntAttribute("scaleH");

    XMLElement* pages = root->FirstChildElement("pages");
    this->imageName = pages->FirstChildElement("page")->Attribute("file");

    // create font and texture
    this->createFont(file);

    XMLElement* chars = root->FirstChildElement("chars");
    XMLElement* charElement = chars->FirstChildElement("char");
    while(charElement) {

        int x, y, offsetX, offsetY, width, height, advanceX;

        offsetX = charElement->IntAttribute("xoffset");
        offsetY = -charElement->IntAttribute("yoffset");
        advanceX = charElement->IntAttribute("xadvance");

        x = charElement->IntAttribute("x");
        y = charElement->IntAttribute("y");
        width = charElement->IntAttribute("width");
        height = charElement->IntAttribute("height");

        Rect region = Rect(x, y, width, height);

        const char* letter = charElement->Attribute("letter");
        int charID = charElement->IntAttribute("id");

        Glyph::ptr glyph = Glyph::create();
        glyph->init(charID, offsetX, offsetY, advanceX, 0);

        TextureFrame::ptr frame = TextureFrame::create();
        frame->init(region);
        frame->setName(letter);
        frame->setTexture(this->texture);
        glyph->setTextureFrame(frame);

        this->font->addGlyph(glyph);

        charElement = charElement->NextSiblingElement();
    }
}

TexturePtr GetTextureByImageFile(FilePtr& imageFile)
{
    const std::string& path = imageFile->getName();
    int pos2 = path.find_last_of(".");
    auto name = path.substr(0, pos2);

    TexturePtr texture;
    if (TextureManager::getInstance().containTexture(name)) {
        texture = TextureManager::getInstance().getTexture(name);
    } else {
        texture = Texture::create();
        texture->setFile(imageFile);

        TextureManager::getInstance().registerTexture(texture);
    }

    return texture;
}

extern FilePtr GetImageFile(const FilePtr& atlasFile, const string &imageName);

void AngleFontParser::createFont(FilePtr& file)
{
    this->font = Create<Font>(FontType::Bitmap, this->size, file);
    this->font->setName(this->family);
    FontManager::getInstance().Register(this->font);

    FilePtr imageFile = GetImageFile(file, this->imageName);
    this->texture = GetTextureByImageFile(imageFile);
}

} // namespace re
