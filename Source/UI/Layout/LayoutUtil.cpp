
#include "LayoutUtil.h"
#include "UI/Base/Transform2D.h"

namespace re {

Vec2 LayoutUtil::AlignTypeToPoint(AlignType type) {
    switch (type) {
        case AlignType::CENTER:
            return Vec2(0.5, 0.5);
            break;
        case AlignType::CENTER_TOP:
            return Vec2(0.5, 1.0);
            break;
        case AlignType::CENTER_BOTTOM:
            return Vec2(0.5, 0.0);
            break;
        case AlignType::LEFT_TOP:
            return Vec2(0.0, 1.0);
            break;
        case AlignType::LEFT_CENTER:
            return Vec2(0.0, 0.5);
            break;
        case AlignType::LEFT_BOTTOM:
            return Vec2(0.0, 0.0);
            break;
        case AlignType::RIGHT_TOP:
            return Vec2(1.0, 1.0);
            break;
        case AlignType::RIGHT_CENTER:
            return Vec2(1.0, 0.5);
            break;
        case AlignType::RIGHT_BOTTOM:
            return Vec2(1.0, 0.0);
            break;
    }

    return Vec2();
}

Vec2 LayoutUtil::GetBoundBoxPosition(Transform2DPtr node, AlignType type) {
    float x = 0, y = 0;
    
    Rect rect = node->getBoundingBox();
    
    switch (type) {
        case AlignType::CENTER:
            x = (rect.getMinX() + rect.getMaxX()) / 2.0;
            y = (rect.getMinY() + rect.getMaxY()) / 2.0;
            break;
        case AlignType::CENTER_BOTTOM:
            x = (rect.getMinX() + rect.getMaxX()) / 2.0;
            y = rect.getMinY();
            break;
        case AlignType::CENTER_TOP:
            x = (rect.getMinX() + rect.getMaxX()) / 2.0;
            y = rect.getMaxY();
            break;
            
        default:
            break;
    }
    
    return Vec2(x, y);
}

void LayoutUtil::Layout(Transform2DPtr view, AlignType from, Transform2DPtr target, AlignType to, float offsetX, float offsetY) {
    Vec2 fromV = AlignTypeToPoint(from);
    Vec2 toV = AlignTypeToPoint(to);
    
    bool targetIsParent = false;
    if (view->getComponentInParent<Transform2D>() == target) {
        targetIsParent = true;
    }
    
    Layout(view, fromV, target, toV, targetIsParent, Vec2(offsetX, offsetY));
}

void LayoutUtil::LayoutToParent(Transform2DPtr view, AlignType from, AlignType to, float offsetX, float offsetY) {
    Vec2 fromV = AlignTypeToPoint(from);
    Vec2 toV = AlignTypeToPoint(to);

    LayoutToParent(view, fromV, toV, Vec2(offsetX, offsetY));
}

void LayoutUtil::Layout(Transform2DPtr src, const Vec2 &viewAlign, Transform2DPtr target, const Vec2 &targetAlign, bool targetIsParent /* = false */, const Vec2 &offset) {

    Vec2 result = CalculateLayout(src, viewAlign, target, targetAlign, targetIsParent, offset);

    src->setPosition(result);
}

void LayoutUtil::LayoutToParent(Transform2DPtr view, const Vec2 &viewAlign, const Vec2 &targetAlign, const Vec2 &offset)
{
    Layout(view, viewAlign, view->getComponentInParent<Transform2D>(), targetAlign, true, offset);
}

Vec2 LayoutUtil::CalculateLayout(Transform2DPtr view, AlignType from, Transform2DPtr target, AlignType to, float offsetX, float offsetY)
{
    Vec2 fromV = AlignTypeToPoint(from);
    Vec2 toV = AlignTypeToPoint(to);
    Vec2 offset = Vec2(offsetX, offsetY);

    bool targetIsParent = false;
    if (view->getComponentInParent<Transform2D>() == target) {
        targetIsParent = true;
    }

    return CalculateLayout(view, fromV, target, toV, targetIsParent, offset);
}

Vec2 LayoutUtil::CalculateLayout(Transform2DPtr view, const Vec2 &viewAlign, Transform2DPtr target, const Vec2 &targetAlign, bool targetIsParent, const Vec2 &offset)
{
    const Vec2& srcAnchorPoint = view->getAnchorPoint();
    const Vec2 anchorPointDiff = viewAlign - srcAnchorPoint;
    const Vec2& targetAnchorPoint = target->getAnchorPoint();
    float targetAlignXPosition,targetAlignYPosition;
    if (targetIsParent) {
        targetAlignXPosition = target->getSize().width * targetAlign.x;
        targetAlignYPosition = target->getSize().height * targetAlign.y;
    } else {
        const Vec2 position = target->getPosition();
        const Vec2 targetAnchorPointDiff = targetAlign - targetAnchorPoint;
        targetAlignXPosition = position.x + target->getBoundingBox().size.width * targetAnchorPointDiff.x;
        targetAlignYPosition = position.y + target->getBoundingBox().size.height * targetAnchorPointDiff.y;
    }

    Vec2 result = Vec2(targetAlignXPosition - anchorPointDiff.x * view->getBoundingBox().size.width + offset.x,
                       targetAlignYPosition - anchorPointDiff.y * view->getBoundingBox().size.height + offset.y);

    return result;
}

Vec2 LayoutUtil::CalculateLayoutToParent(Transform2DPtr view, AlignType from, AlignType to, float offsetX, float offsetY)
{
    return CalculateLayout(view, from, view->getComponentInParent<Transform2D>(), to, offsetX, offsetY);
}

Vec2 LayoutUtil::CalculateLayoutToParent(Transform2DPtr view, const Vec2 &viewAlign, const Vec2 &targetAlign, const Vec2 &offset)
{
    return CalculateLayout(view, viewAlign, view->getComponentInParent<Transform2D>(), targetAlign, true, offset);
}

}
