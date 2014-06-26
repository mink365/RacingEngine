#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <vector>

#include "Font.h"

namespace re {

class FontManager
{
public:
    FontManager();

public:
    void registerFont(Font::ptr& font);
    void desposeFont(Font::constPtr& font);

    bool containFont(std::string name);
    Font::ptr getFont(std::string name);

private:
    std::vector<Font::ptr> fonts;
};

}

#endif // FONTMANAGER_H
