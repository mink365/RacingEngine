#ifndef TEXTUREUNITSTATE_H
#define TEXTUREUNITSTATE_H

#include <vector>
#include "Texture/Texture.h"
#include "Base/Shared.h"
#include "Base/Clonable.h"
#include "Math/Vector.h"

namespace re {

class Pass;

class TextureUnitState : public Shared<TextureUnitState>, public Clonable<TextureUnitState>
{
    friend class Pass;

public:
    TextureUnitState();

    float getRotation() const;
    void setRotation(float value);

    const Vec2& getOffset() const;
    void setOffset(float u, float v);

    const Vec2& getScale() const;
    void setScale(float u, float v);

    void setUVstate(float offsetU, float offsetV, float scaleU, float scaleV, float rotation);

    Texture::ptr getActivityTexture();
    void addTextureFrame(Texture::ptr texture);

    TextureUnitState::ptr clone() const override;

private:
    std::weak_ptr<Pass> pass;
    std::vector<Texture::ptr> frames;

    int activeFrameIndex;

    Vec2 offset;
    Vec2 scale;
    float rotation;
};

} // namespace re

#endif // TEXTUREUNITSTATE_H
