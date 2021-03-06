#include "TextureFrame.h"

#include "Math/Matrix.h"

namespace re {

TextureFrame::TextureFrame()
{
}

TexturePtr TextureFrame::getTexture()
{
    return this->texture;
}

void TextureFrame::setTexture(TexturePtr texture)
{
    this->texture = texture;
}

void TextureFrame::init(const Rect &region)
{
    this->isTrimmed = false;
    this->isRotated = false;

    this->region = region;

    this->originalSize = region.size;
}

void TextureFrame::init(const string &name, bool rotated, float x, float y, float width, float height)
{
    this->name = name;

    this->isTrimmed = false;
    this->isRotated = rotated;

    this->region = {x, y, width, height};

    this->originalSize = {width, height};
}

void TextureFrame::init(const string &name, bool rotated, float x, float y, float width, float height, float offsetX, float offsetY, float originalW, float originalH)
{
    this->name = name;

    this->isTrimmed = true;
    this->isRotated = rotated;

    this->region = {x, y, width, height};
    this->offset = {offsetX, offsetY};
    this->originalSize = {originalW, originalH};
}

Uv TextureFrame::getUv(const Vec2 &v) const
{
    Vec2 p = this->transform(v);

    return this->texture->getUv(p);
}

const Size &TextureFrame::getSize() const
{
    return this->region.size;
}

const Vec2 &TextureFrame::getOffset() const
{
    return this->offset;
}

const Size &TextureFrame::getOriginalSize() const
{
    return this->originalSize;
}

Vec2 TextureFrame::transform(const Vec2 &ov) const
{
    // translate to pic coord
    Vec2 v = {ov.x, this->originalSize.height - ov.y};

//    Mat4 translateM, rotateM, textureTranslateM;

//    if (this->isTrimmed) {
//        translateM.setTranslation(offset.x, offset.x, 0);
//    }

//    textureTranslateM.setTranslation(region.origin.x, region.origin.y, 0);

//    if (this->isRotated) {
//        rotateM.setRotationZ(-PI / 2.0f);
//    }

//    Mat4 m = translateM * rotateM  * textureTranslateM;

//    Vec3 p(v.x, v.y, 0);

//    Vec3 result =  m * p;

//    return Vec2(result.x, result.y);

    /* same as java version
    // input is GL
    Vec2 source = {v.x, this->originalSize.height - v.y};

    Vec2 p;
    if (this->isRotated) {
        p.y = this->region.origin.y - this->offset.x + source.x;
        p.x = this->region.origin.x + (this->offset.y + this->region.size.height) - source.y;
    } else {
        p.x = this->region.origin.x - this->offset.x + source.x;
        p.y = this->region.origin.y - this->offset.y + source.y;
    }

    Uv uv = this->texture->getUv(p);
    */


    if (this->isTrimmed) {
        v = {v.x - offset.x, v.y - offset.y};
    }

    if (this->isRotated) {
        v = {region.size.height - v.y, v.x};
    }

    v = v + region.origin;

    return v;
}

}
