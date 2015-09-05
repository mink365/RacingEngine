#ifndef RE_UI_TEXT_H
#define RE_UI_TEXT_H

#include "Graphic.h"
#include "Font/Font.h"

namespace re {
namespace ui {

class Text : public Graphic
{
public:
    Text();

    void setFont(Font::ptr& font);

    void setText(const string &text);

protected:
    void onAwake();
    void registerEvents();

protected:
    void copyProperties(const Text& rhs);

protected:
    Font::ptr font;

    std::string text;
    bool dirtyFlag;
};

} // namespace ui
} // namespace re

#endif // RE_UI_TEXT_H
