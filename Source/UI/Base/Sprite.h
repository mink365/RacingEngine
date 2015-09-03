#ifndef RE_UI_SPRITE_H
#define RE_UI_SPRITE_H

#include "Texture/Frame/TextureFrame.h"
#include "Graphic.h"

namespace re {
namespace ui {

class Sprite : public Graphic
{
public:
    Sprite()
        : Graphic()
    {};

    void init();
    void init(const std::string& tex);

public:
    void setFrame(TextureFrame::ptr frame);
    void rebind();

    virtual void updateViewColor();

protected:
    void copyProperties(const Sprite &rhs);

private:
    Rect rect;

    TextureFrame::ptr frame;
};

} // namespace ui
} // namespace re

#endif // RE_UI_SPRITE_H
