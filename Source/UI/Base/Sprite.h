#ifndef SPRITE_H
#define SPRITE_H

#include "Texture/Frame/TextureFrame.h"
#include "Node2d.h"

namespace re {

class Sprite : public Node2d
{
public:
    Sprite() {};
    Sprite(const std::string& tex);
    Sprite(const std::string& tex, const Rect& rect);

    void rebind();

protected:
    void init(const TextureFrame::ptr& tex, const Rect& rect);

protected:
    virtual NodePtr createCloneInstance() const;
    virtual void copyProperties(const Node* node) override;

private:
    Rect rect;

    TextureFrame::ptr frame;
};

}

#endif // SPRITE_H
