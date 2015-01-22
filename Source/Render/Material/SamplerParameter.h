#ifndef TEXTUREUNITSTATE_H
#define TEXTUREUNITSTATE_H

#include <vector>
#include "Texture/Texture.h"
#include "Base/Shared.h"
#include "Base/Clonable.h"
#include "Math/Vector.h"

namespace re {

class Pass;

class SamplerParameter : public Shared<SamplerParameter, string>, public Clonable<SamplerParameter>
{
    friend class Material;

public:
    SamplerParameter(const string &name);

    void setUVstate(const Vec2& offset, const Vec2& scale, float rotation);

    const string& getName() const;

    const Vec2& getOffset() const;
    void setOffset(const Vec2& rhs);

    const Vec2& getScale() const;
    void setScale(const Vec2& rhs);

    float getRotation() const;
    void setRotation(float value);

    Texture::ptr getTexture();
    void setTexture(TexturePtr& tex);

    SamplerParameter::ptr clone() const override;

private:
    std::string name;

    Texture::ptr texture;

    Vec2 offset;
    Vec2 scale;
    float rotation;
};

} // namespace re

#endif // TEXTUREUNITSTATE_H
