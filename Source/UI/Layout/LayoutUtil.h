#ifndef __LayoutUtil__
#define __LayoutUtil__

#include "UI/Base/Node2d.h"

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

namespace LayoutUtil {
    Vec2 AlignTypeToPoint(AlignType type);
    
    Vec2 GetBoundBoxPosition(Node2d::ptr node, AlignType type);
    
    void LayoutLeft(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutRight(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutTop(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutBottom(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutCenter(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutLeftTop(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutRightTop(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutLeftBottom(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutRightBottom(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);

    void LayoutToParentLeft(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentRight(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentTop(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentBottom(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentCenter(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentLeftTop(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentRightTop(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentLeftBottom(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentRightBottom(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    
    void Layout(Node2d::ptr view, AlignType from, Node2d::ptr target, AlignType to, float offsetx = 0.0, float offsetY = 0.0);
    void LayoutToParent(Node2d::ptr view, AlignType from, AlignType to, float offsetx = 0.0, float offsetY = 0.0);
    
    void Layout(Node2d::ptr view, float viewAlignX, float viewAlignY, Node2d::ptr target,
                float targetAlignX, float targetAlignY, bool targetIsParent = false, float offsetX = 0.0, float offsetY = 0.0);
    
}

}

#endif /* defined(__LayoutUtil__) */
