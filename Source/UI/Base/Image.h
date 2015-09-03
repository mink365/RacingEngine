#ifndef RE_UI_IMAGE_H
#define RE_UI_IMAGE_H

#include "Texture/Frame/TextureFrame.h"
#include "Graphic.h"

namespace re {
namespace ui {

class Image : public Graphic
{
public:
    Image()
        : Graphic()
    {};

    void init();
    void init(const std::string& tex);

public:
    void setFrame(TextureFrame::ptr frame);
    void rebind();

    virtual void updateViewColor();

protected:
    void copyProperties(const Image &rhs);

private:
    Rect rect;

    TextureFrame::ptr frame;
};

} // namespace ui
} // namespace re

#endif // RE_UI_IMAGE_H
