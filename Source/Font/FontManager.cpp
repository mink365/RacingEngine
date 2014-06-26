#include "FontManager.h"

#include <algorithm>

namespace re {

FontManager::FontManager()
{
}

void FontManager::registerFont(Font::ptr &font)
{
    this->fonts.push_back(font);
}

void FontManager::desposeFont(Font::constPtr &font)
{
    auto iter = std::find(this->fonts.begin(), this->fonts.end(), font);

    if (iter != this->fonts.end()) {
        this->fonts.erase(iter);
    }
}

bool FontManager::containFont(string name)
{
    auto iter = std::find_if(this->fonts.begin(), this->fonts.end(), [=](Font::ptr font) {
        if (font->getName() == name) {
            return true;
        }

        return false;
    });

    if (iter != this->fonts.end()) {
        return true;
    }

    return false;
}

Font::ptr FontManager::getFont(string name)
{
    auto iter = std::find_if(this->fonts.begin(), this->fonts.end(), [=](Font::ptr font) {
        if (font->getName() == name) {
            return true;
        }

        return false;
    });

    if (iter != this->fonts.end()) {
        return *iter;
    }

    return nullptr;
}

}
