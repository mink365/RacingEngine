#include "QuadStuffer.h"

namespace re {

QuadStuffer::QuadStuffer()
{
}

void QuadStuffer::FillQuad(const TextureFrame::constPtr &frame, const Size &size, const Color &color, const Geometry::ptr &geomerty)
{
    Rect rect(0, 0, size.width, size.height);

    const Size& originalSize = frame->getOriginalSize();
    Rect textureRect(0, 0, originalSize.width, originalSize.height);

    geomerty->clear();
    AddOriginalQuad(rect, textureRect, color, frame, geomerty);
}


Vec2 GetPoint(const Rect& rect, AlignType align) {
  switch (align) {
  case AlignType::LEFT_TOP:
    return Vec2(rect.getMinX(), rect.getMaxY());
  case AlignType::LEFT_BOTTOM:
    return Vec2(rect.getMinX(), rect.getMinY());
  case AlignType::RIGHT_TOP:
    return Vec2(rect.getMaxX(), rect.getMaxY());
  case AlignType::RIGHT_BOTTOM:
    return Vec2(rect.getMaxX(), rect.getMinY());
  case AlignType::CENTER:
      return Vec2(rect.getMidX(), rect.getMidY());
  case AlignType::CENTER_BOTTOM:
      return Vec2(rect.getMidX(), rect.getMinY());
  case AlignType::CENTER_TOP:
      return Vec2(rect.getMidX(), rect.getMaxY());
  case AlignType::LEFT_CENTER:
      return Vec2(rect.getMinX(), rect.getMidY());
  case AlignType::RIGHT_CENTER:
      return Vec2(rect.getMaxX(), rect.getMidY());
  }

  return Vec2();
}

typedef float (Rect::*PointFunc)() const;
typedef float (Rect::*DurFunc)() const;
typedef void (Rect::*SetFunc)(float);

float GetVertex(const Rect& newTextureRect, const Rect& oldTextureRect, const Rect& oldVertexRect, PointFunc pf, DurFunc df) {
    float diffTexture = (newTextureRect.*pf)() - (oldTextureRect.*pf)();
    float diffVertex = (diffTexture / (oldTextureRect.*df)()) * (oldVertexRect.*df)();

    return (oldVertexRect.*pf)() + diffVertex;
}

void SetVertexAndTexture(const Rect& clampRect, Rect& newTextureRect, Rect& vertexRect, const Rect& oldTextureRect, PointFunc pf, DurFunc df, SetFunc sf) {
    (newTextureRect.*sf)((clampRect.*pf)());

    float v = GetVertex(newTextureRect, oldTextureRect, vertexRect, pf, df);

    (newTextureRect.*sf)((newTextureRect.*pf)());
    (vertexRect.*sf)(v);
}

/**
 * @brief Clamp
 * @param clampRect
 * @param textureRect
 * @param vertexRect
 *
 * use use the texture in not trimmed space, so if the textureFrame is trimmed, textureRect
 * and vertextRect from use need to be clamp to the trimmed space
 */
void Clamp(const Rect& clampRect, Rect& textureRect, Rect& vertexRect) {
    if (clampRect.containsRect(textureRect)) {

    } else if (!clampRect.intersectsRect(textureRect)) {
        const Rect oldTextureRect = textureRect;

        if (oldTextureRect.getMaxX() <= clampRect.getMinX()) {
            textureRect.setLeft(clampRect.getMinX());
            textureRect.setRight(clampRect.getMinX());

            vertexRect.size.width = 0;
        } else if (oldTextureRect.getMinX() >= clampRect.getMaxX()) {
            textureRect.setLeft(clampRect.getMaxX());
            textureRect.setRight(clampRect.getMaxX());

            vertexRect.size.width = 0;
        }

        if (oldTextureRect.getMaxY() <= clampRect.getMinY()) {
            textureRect.setBottom(clampRect.getMinY());
            textureRect.setTop(clampRect.getMinY());

            vertexRect.size.height = 0;
        } else if (oldTextureRect.getMinY() >= clampRect.getMaxY()) {
            textureRect.setBottom(clampRect.getMaxY());
            textureRect.setTop(clampRect.getMaxY());

            vertexRect.size.height = 0;
        }

    } else {
        Rect oldTextureRect = textureRect;

        if (textureRect.getMinX() < clampRect.getMinX()) {
            SetVertexAndTexture(clampRect, textureRect, oldTextureRect, vertexRect,
                                &Rect::getMinX, &Rect::getWidth, &Rect::setLeft);
        }

        if (textureRect.getMaxX() > clampRect.getMaxX()) {
            SetVertexAndTexture(clampRect, textureRect, oldTextureRect, vertexRect,
                                &Rect::getMaxX, &Rect::getWidth, &Rect::setRight);
        }

        if (textureRect.getMinY() < clampRect.getMinY()) {
            SetVertexAndTexture(clampRect, textureRect, oldTextureRect, vertexRect,
                                &Rect::getMinY, &Rect::getHeight, &Rect::setBottom);
        }

        if (textureRect.getMaxY() > clampRect.getMaxY()) {
            SetVertexAndTexture(clampRect, textureRect, oldTextureRect, vertexRect,
                                &Rect::getMaxY, &Rect::getHeight, &Rect::setTop);
        }
    }
}

void QuadStuffer::AddOriginalQuad(const Rect &rect, const Rect &textureRect, const Color &color, const TextureFrame::constPtr& frame, const Geometry::ptr &geomerty)
{
    // clamp texture
    Rect clampRect(frame->getOffset(), frame->getSize());

    Rect newTextureRect = textureRect;
    Rect newVertexRect = rect;

    Clamp(clampRect, newTextureRect, newVertexRect);

    // 0
    AddVertex(newVertexRect, newTextureRect, color, AlignType::LEFT_TOP, frame, geomerty);
    AddVertex(newVertexRect, newTextureRect, color, AlignType::LEFT_BOTTOM, frame, geomerty);
    AddVertex(newVertexRect, newTextureRect, color, AlignType::RIGHT_BOTTOM, frame, geomerty);
    AddVertex(newVertexRect, newTextureRect, color, AlignType::RIGHT_TOP, frame, geomerty);

    int faceIndex = geomerty->getFaces().size() * 3 / 6;
    int offset = faceIndex * 4;
    Face face(offset + 0, offset + 1, offset + 2);
    Face face2(offset + 2, offset + 3, offset + 0);
    geomerty->addFace(face);
    geomerty->addFace(face2);
}

void QuadStuffer::AddVertex(const Rect& rect, const Rect& textureRect, const Color& color, AlignType align, const TextureFrame::constPtr& frame, const Geometry::ptr& geomerty)
{
    Vec2 point = GetPoint(rect, align);
    Vec2 texturePoint = GetPoint(textureRect, align);

    Uv uv = frame->getUv(texturePoint);

    AddVertex(point, uv, color, geomerty);
}

void QuadStuffer::AddVertex(const Vec2 &p, const Uv &uv, const Color &color, const Geometry::ptr &geomerty)
{
    Vertex v;

    v.xyz.set(p.x, p.y, 0);
    v.uv = uv;
    v.color = color;

    geomerty->addVertex(v);
}

}
