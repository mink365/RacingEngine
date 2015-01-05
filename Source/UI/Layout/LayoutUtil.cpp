
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

Vec2 LayoutUtil::GetBoundBoxPosition(Node2d::ptr node, AlignType type) {
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

void LayoutUtil::LayoutLeft(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 1, 0.5, target, 0, 0.5, false, offsetX, offsetY);
}

void LayoutUtil::LayoutRight(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0, 0.5, target, 1, 0.5, false, offsetX, offsetY);
}

void LayoutUtil::LayoutTop(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0.5, 0, target, 0.5, 1, false, offsetX, offsetY);
}

void LayoutUtil::LayoutBottom(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0.5, 1, target, 0.5, 0, false, offsetX, offsetY);
}

void LayoutUtil::LayoutCenter(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0.5, 0.5, target, 0.5, 0.5, false, offsetX, offsetY);
}

void LayoutUtil::LayoutLeftTop(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 1, 1, target, 0, 1, false, offsetX, offsetY);
}

void LayoutUtil::LayoutRightTop(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0, 1, target, 1, 1, false, offsetX, offsetY);
}

void LayoutUtil::LayoutLeftBottom(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 1, 0, target, 0, 0, false, offsetX, offsetY);
}

void LayoutUtil::LayoutRightBottom(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0, 0, target, 1, 0, false, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentLeft(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0, 0.5, view->getParent(), 0, 0.5, true, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentRight(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 1, 0.5, view->getParent(), 1, 0.5, true, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentTop(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0.5, 1, view->getParent(), 0.5, 1, true, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentBottom(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0.5, 0, view->getParent(), 0.5, 0, true, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentCenter(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0.5, 0.5, view->getParent(), 0.5, 0.5, true, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentLeftTop(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0, 1, view->getParent(), 0, 1, true, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentRightTop(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 1, 1, view->getParent(), 1, 1, true, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentLeftBottom(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 0, 0, view->getParent(), 0, 0, true, offsetX, offsetY);
}

void LayoutUtil::LayoutToParentRightBottom(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    Layout(view, 1, 0, view->getParent(), 1, 0, true, offsetX, offsetY);
}

void LayoutUtil::Layout(Node2d::ptr view, AlignType from, Node2d::ptr target, AlignType to, float offsetX, float offsetY) {
    Vec2 fromV = AlignTypeToPoint(from);
    Vec2 toV = AlignTypeToPoint(to);
    
    bool targetIsParent = false;
    if (view->getParent() == target) {
        targetIsParent = true;
    }
    
    Layout(view, fromV.x, fromV.y, target, toV.x, toV.y, targetIsParent, offsetX, offsetY);
}

void LayoutUtil::LayoutToParent(Node2d::ptr view, AlignType from, AlignType to, float offsetx, float offsetY) {
    Layout(view, from, view->getParent(), to, offsetx, offsetY);
}

void LayoutUtil::Layout(Node2d::ptr src, float srcAlignX, float srcAlignY, Node2d::ptr target,
                        float targetAlignX, float targetAlignY, bool targetIsParent /* = false */, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    const Vec2& srcAnchorPoint = src->getAnchorPoint();
    const Vec2& anchorPointDiff = Vec2(srcAlignX - srcAnchorPoint.x, srcAlignY - srcAnchorPoint.y);
    const Vec2& targetAnchorPoint = target->getAnchorPoint();
    float targetAlignXPosition,targetAlignYPosition;
    if (targetIsParent) {
        targetAlignXPosition = target->getContentSize().width * targetAlignX;
        targetAlignYPosition = target->getContentSize().height * targetAlignY;
    } else {
        const Vec2 position = target->getTransform2D()->getPosition();
        targetAlignXPosition = position.x + target->getBoundingBox().size.width * (targetAlignX - targetAnchorPoint.x);
        targetAlignYPosition = position.y + target->getBoundingBox().size.height * (targetAlignY - targetAnchorPoint.y);
    }
    src->getTransform2D()->setPosition(Vec2(targetAlignXPosition - anchorPointDiff.x * src->getBoundingBox().size.width + offsetX,
                         targetAlignYPosition - anchorPointDiff.y * src->getBoundingBox().size.height + offsetY));

}

}
