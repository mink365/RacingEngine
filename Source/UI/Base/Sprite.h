#ifndef SPRITE_H
#define SPRITE_H

#include "Texture/Frame/TextureFrame.h"
#include "Node2d.h"

namespace re {

class Sprite : public Node2d
{
public:
    Sprite(const std::string& tex);
    Sprite(const std::string& tex, const Rect& rect);

protected:
    void init(const TextureFrame::ptr& tex, const Rect& rect);

private:
    Rect rect;

    TextureFrame::ptr texture;
};

}

#endif // SPRITE_H
