#ifndef SPRITE_H
#define SPRITE_H

#include "Texture/Frame/TextureFrame.h"
#include "Component.h"

namespace re {

class Sprite : public Component
{
public:
    Sprite() {};

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

}

#endif // SPRITE_H
