#ifndef __LayoutUtil__
#define __LayoutUtil__

#include "UI/Base/Transform2D.h"

namespace re {

enum class AlignType {
    CENTER,
    CENTER_TOP,
    CENTER_BOTTOM,
    LEFT_TOP,
    LEFT_CENTER,
    LEFT_BOTTOM,
    RIGHT_TOP,
    RIGHT_CENTER,
    RIGHT_BOTTOM,
};

enum class Corner {
    UpperLeft,
    UpperRight,
    LowLeft,
    LowRight,
};

enum class Axis {
    Horizontal,
    Vertical,
};

namespace LayoutUtil {
    Vec2 AlignTypeToPoint(AlignType type);
    
    Vec2 GetBoundBoxPosition(Transform2DPtr node, AlignType type);
    
    void Layout(Transform2DPtr view, AlignType from, Transform2DPtr target, AlignType to, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParent(Transform2DPtr view, AlignType from, AlignType to, float offsetX = 0.0, float offsetY = 0.0);
    
    void Layout(Transform2DPtr view, const Vec2& viewAlign, Transform2DPtr target, const Vec2& targetAlign, bool targetIsParent = false, const Vec2& offset=Vec2::Zero);
    void LayoutToParent(Transform2DPtr view, const Vec2& viewAlign, const Vec2& targetAlign, const Vec2& offset=Vec2::Zero);

    Vec2 CalculateLayoutToParent(Transform2DPtr view, AlignType from, AlignType to, float offsetX = 0.0, float offsetY = 0.0);
    Vec2 CalculateLayoutToParent(Transform2DPtr view, const Vec2& viewAlign, const Vec2& targetAlign, const Vec2& offset=Vec2::Zero);
    Vec2 CalculateLayout(Transform2DPtr view, AlignType from, Transform2DPtr target, AlignType to, float offsetX = 0.0, float offsetY = 0.0);
    Vec2 CalculateLayout(Transform2DPtr view, const Vec2& viewAlign, Transform2DPtr target, const Vec2& targetAlign, bool targetIsParent = false, const Vec2& offset=Vec2::Zero);
}

}

#endif /* defined(__LayoutUtil__) */
