
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

void LayoutUtil::LayoutLeft(Transform2DPtr view, Transform2DPtr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 1, 0.5, target, 0, 0.5, false, offsetX, offsetY);
}

void LayoutUtil::LayoutRight(Transform2DPtr view, Transform2DPtr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0, 0.5, target, 1, 0.5, false, offsetX, offsetY);
}

void LayoutUtil::LayoutTop(Transform2DPtr view, Transform2DPtr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0.5, 0, target, 0.5, 1, false, offsetX, offsetY);
}

void LayoutUtil::LayoutBottom(Transform2DPtr view, Transform2DPtr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0.5, 1, target, 0.5, 0, false, offsetX, offsetY);
}

void LayoutUtil::LayoutCenter(Transform2DPtr view, Transform2DPtr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0.5, 0.5, target, 0.5, 0.5, false, offsetX, offsetY);
}

void LayoutUtil::LayoutLeftTop(Transform2DPtr view, Transform2DPtr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 1, 1, target, 0, 1, false, offsetX, offsetY);
}

void LayoutUtil::LayoutRightTop(Transform2DPtr view, Transform2DPtr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0, 1, target, 1, 1, false, offsetX, offsetY);
}

void LayoutUtil::LayoutLeftBottom(Transform2DPtr view, Transform2DPtr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 1, 0, target, 0, 0, false, offsetX, offsetY);
}

void LayoutUtil::LayoutRightBottom(Transform2DPtr view, Transform2DPtr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0, 0, target, 1, 0, false, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentLeft(Transform2DPtr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    LayoutToParent(view, 0, 0.5, 0, 0.5, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentRight(Transform2DPtr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    LayoutToParent(view, 1, 0.5, 1, 0.5, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentTop(Transform2DPtr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    LayoutToParent(view, 0.5, 1, 0.5, 1, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentBottom(Transform2DPtr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    LayoutToParent(view, 0.5, 0, 0.5, 0, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentCenter(Transform2DPtr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    LayoutToParent(view, 0.5, 0.5, 0.5, 0.5, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentLeftTop(Transform2DPtr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    LayoutToParent(view, 0, 1, 0, 1, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentRightTop(Transform2DPtr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    LayoutToParent(view, 1, 1, 1, 1, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentLeftBottom(Transform2DPtr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    LayoutToParent(view, 0, 0, 0, 0, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentRightBottom(Transform2DPtr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    LayoutToParent(view, 1, 0, 1, 0, offsetX, offsetY);
}

void LayoutUtil::Layout(Transform2DPtr view, AlignType from, Transform2DPtr target, AlignType to, float offsetX, float offsetY) {
    Vec2 fromV = AlignTypeToPoint(from);
    Vec2 toV = AlignTypeToPoint(to);
    
    bool targetIsParent = false;
    if (view->getComponentInParent<Transform2D>() == target) {
        targetIsParent = true;
    }
    
    Layout(view, fromV.x, fromV.y, target, toV.x, toV.y, targetIsParent, offsetX, offsetY);
}

void LayoutUtil::LayoutToParent(Transform2DPtr view, AlignType from, AlignType to, float offsetx, float offsetY) {
    Vec2 fromV = AlignTypeToPoint(from);
    Vec2 toV = AlignTypeToPoint(to);

    LayoutToParent(view, fromV.x, fromV.y, toV.x, toV.y, offsetx, offsetY);
}

void LayoutUtil::Layout(Transform2DPtr src, float srcAlignX, float srcAlignY, Transform2DPtr target,
                        float targetAlignX, float targetAlignY, bool targetIsParent /* = false */, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    const Vec2& srcAnchorPoint = src->getAnchorPoint();
    const Vec2& anchorPointDiff = Vec2(srcAlignX - srcAnchorPoint.x, srcAlignY - srcAnchorPoint.y);
    const Vec2& targetAnchorPoint = target->getAnchorPoint();
    float targetAlignXPosition,targetAlignYPosition;
    if (targetIsParent) {
        targetAlignXPosition = target->getContentSize().width * targetAlignX;
        targetAlignYPosition = target->getContentSize().height * targetAlignY;
    } else {
        const Vec2 position = target->getPosition();
        targetAlignXPosition = position.x + target->getBoundingBox().size.width * (targetAlignX - targetAnchorPoint.x);
        targetAlignYPosition = position.y + target->getBoundingBox().size.height * (targetAlignY - targetAnchorPoint.y);
    }
    src->setPosition(Vec2(targetAlignXPosition - anchorPointDiff.x * src->getBoundingBox().size.width + offsetX,
                         targetAlignYPosition - anchorPointDiff.y * src->getBoundingBox().size.height + offsetY));

}

void LayoutUtil::LayoutToParent(Transform2DPtr view, float viewAlignX, float viewAlignY, float targetAlignX, float targetAlignY, float offsetX, float offsetY)
{
    Layout(view, viewAlignX, viewAlignY, view->getComponentInParent<Transform2D>(), targetAlignX, targetAlignY, true, offsetX, offsetY);
}

}
