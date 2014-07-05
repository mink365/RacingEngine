#ifndef TEXTUREFRAME_H
#define TEXTUREFRAME_H

#include "Base/Named.h"
#include "Base/Shared.h"
#include "Math/BoundingVolume.h"
#include "Math/UV.h"

#include "Texture/Texture.h"

namespace re {

class TextureFrame : public Named, public Shared<TextureFrame>
{
public:
    TextureFrame();

    std::shared_ptr<Texture> getTexture();
    void setTexture(std::shared_ptr<Texture> texture);

    void init(const Rect& region);
    void init(const string& name, bool rotated, float x, float y, float width, float height);
    void init(const string& name, bool rotated, float x, float y, float width, float height, float offsetX, float offsetY, float originalW, float originalH);

    /**
     * @brief getUv
     * @param v
     * @return
     *
     * get the UV value frome a position in the area of this frame
     */
    Uv getUv(const Vec2& v) const;

    const Size& getSize() const;

    const Vec2& getOffset() const;
    const Size& getOriginalSize() const;

private:
    Vec2 transform(const Vec2& v) const;

private:
    std::shared_ptr<Texture> texture;

    bool isTrimmed;
    bool isRotated;

    Rect region;

    Size originalSize;
    Vec2 offset;
};

}

#endif // TEXTUREFRAME_H
