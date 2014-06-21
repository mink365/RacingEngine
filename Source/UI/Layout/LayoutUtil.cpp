
#include "LayoutUtil.h"

namespace re {

Vec2 LayoutUtil::alignTypeToPoint(AlignType type) {
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
}

Vec2 LayoutUtil::getBoundBoxPosition(Node2d::ptr node, AlignType type) {
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

void LayoutUtil::layoutLeft(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 0.5, target, 0, 0.5, false, offsetX, offsetY);
}

void LayoutUtil::layoutRight(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 0.5, target, 1, 0.5, false, offsetX, offsetY);
}

void LayoutUtil::layoutTop(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 0, target, 0.5, 1, false, offsetX, offsetY);
}

void LayoutUtil::layoutBottom(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 1, target, 0.5, 0, false, offsetX, offsetY);
}

void LayoutUtil::layoutCenter(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 0.5, target, 0.5, 0.5, false, offsetX, offsetY);
}

void LayoutUtil::layoutLeftTop(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 1, target, 0, 1, false, offsetX, offsetY);
}

void LayoutUtil::layoutRightTop(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 1, target, 1, 1, false, offsetX, offsetY);
}

void LayoutUtil::layoutLeftBottom(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 0, target, 0, 0, false, offsetX, offsetY);
}

void LayoutUtil::layoutRightBottom(Node2d::ptr view, Node2d::ptr target, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 0, target, 1, 0, false, offsetX, offsetY);
}

void LayoutUtil::layoutParentLeft(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 0.5, view->getParent(), 0, 0.5, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentRight(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 0.5, view->getParent(), 1, 0.5, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentTop(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 1, view->getParent(), 0.5, 1, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentBottom(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 0, view->getParent(), 0.5, 0, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentCenter(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0.5, 0.5, view->getParent(), 0.5, 0.5, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentLeftTop(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 1, view->getParent(), 0, 1, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentRightTop(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 1, view->getParent(), 1, 1, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentLeftBottom(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 0, 0, view->getParent(), 0, 0, true, offsetX, offsetY);
}

void LayoutUtil::layoutParentRightBottom(Node2d::ptr view, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    layout(view, 1, 0, view->getParent(), 1, 0, true, offsetX, offsetY);
}

void LayoutUtil::layout(Node2d::ptr view, AlignType from, Node2d::ptr target, AlignType to, float offsetX, float offsetY) {
    Vec2 fromV = alignTypeToPoint(from);
    Vec2 toV = alignTypeToPoint(to);
    
    bool targetIsParent = false;
    if (view->getParent() == target) {
        targetIsParent = true;
    }
    
    layout(view, fromV.x, fromV.y, target, toV.x, toV.y, targetIsParent, offsetX, offsetY);
}

void LayoutUtil::layoutParent(Node2d::ptr view, AlignType from, AlignType to, float offsetx, float offsetY) {
    layout(view, from, view->getParent(), to, offsetx, offsetY);
}

void LayoutUtil::layout(Node2d::ptr src, float srcAlignX, float srcAlignY, Node2d::ptr target,
                        float targetAlignX, float targetAlignY, bool targetIsParent /* = false */, float offsetX /* = 0.0 */, float offsetY /* = 0.0 */) {
    const Vec2& srcAnchorPoint = src->getAnchorPoint();
    const Vec2& anchorPointDiff = Vec2(srcAlignX - srcAnchorPoint.x, srcAlignY - srcAnchorPoint.y);
    const Vec2& targetAnchorPoint = target->getAnchorPoint();
    float targetAlignXPosition,targetAlignYPosition;
    if (targetIsParent) {
        targetAlignXPosition = target->getContentSize().width * targetAlignX;
        targetAlignYPosition = target->getContentSize().height * targetAlignY;
    } else {
        targetAlignXPosition = target->getPositionX() + target->getBoundingBox().size.width * (targetAlignX - targetAnchorPoint.x);
        targetAlignYPosition = target->getPositionY() + target->getBoundingBox().size.height * (targetAlignY - targetAnchorPoint.y);
    }
    src->setPosition(Vec2(targetAlignXPosition - anchorPointDiff.x * src->getBoundingBox().size.width + offsetX,
                         targetAlignYPosition - anchorPointDiff.y * src->getBoundingBox().size.height + offsetY));

}

}
