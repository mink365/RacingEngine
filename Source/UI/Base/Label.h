#ifndef RE_UI_LABEL_H
#define RE_UI_LABEL_H

#include "Graphic.h"
#include "Font/Font.h"

namespace re {
namespace ui {

class Label : public Graphic
{
public:
    Label();

    void init(Font::ptr& font);

    void setText(const string &text);

protected:
    void copyProperties(const Label& rhs);

protected:
    Font::ptr font;

    std::string text;
    bool dirtyFlag;
};

} // namespace ui
} // namespace re

#endif // RE_UI_LABEL_H
