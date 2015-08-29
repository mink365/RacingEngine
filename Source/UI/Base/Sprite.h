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

    void init(const std::string& tex);
    void init(const std::string& tex, const Rect& rect);
    void init(const TextureFrame::ptr& tex, const Rect& rect);

public:
    void rebind();

    virtual void updateViewColor();

protected:
    virtual ComponentPtr createCloneInstance() const;
    virtual void copyProperties(const Component* component) override;

private:
    Rect rect;

    TextureFrame::ptr frame;
};

} // namespace ui
} // namespace re

#endif // RE_UI_SPRITE_H
