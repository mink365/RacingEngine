#ifndef QUADSTUFFER_H
#define QUADSTUFFER_H

#include "Math/Vector.h"
#include "Math/UV.h"
#include "Math/Color.h"
#include "Math/BoundingVolume.h"
#include "Geometry/Geometry.h"
#include "Texture/Frame/TextureFrame.h"

#include "UI/Layout/LayoutUtil.h"

namespace re {

class QuadStuffer
{
public:
    QuadStuffer();

    static void FillQuad(const TextureFrame::constPtr& frame, const Size& size, const Geometry::ptr& geomerty);
    static void AddOriginalQuad(const Rect& rect, const Rect& textureRect, const Color& color, const TextureFrame::constPtr& frame, const Geometry::ptr& geomerty);
    static void AddVertex(const Rect& rect, const Rect& textureRect, const Color& color, AlignType align, const TextureFrame::constPtr& frame, const Geometry::ptr& geomerty);
    static void AddVertex(const Vec2& p, const Uv& uv, const Color& color, const Geometry::ptr& geomerty);
};

}

#endif // QUADSTUFFER_H
