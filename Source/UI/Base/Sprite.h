#ifndef SPRITE_H
#define SPRITE_H

#include "Texture/Frame/TextureFrame.h"
#include "Node2d.h"

namespace re {

class Sprite : public Node2d
{
public:
    Sprite() {};

    void init(const std::string& tex);
    void init(const std::string& tex, const Rect& rect);
    void init(const TextureFrame::ptr& tex, const Rect& rect);

public:
    void rebind();

    virtual void updateViewColor() override;

protected:
    virtual NodePtr createCloneInstance() const;
    virtual void copyProperties(const Node* node) override;

private:
    Rect rect;

    TextureFrame::ptr frame;
};

}

#endif // SPRITE_H
