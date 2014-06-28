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
    Vec2 alignTypeToPoint(AlignType type);
    
    Vec2 getBoundBoxPosition(Node2d::ptr node, AlignType type);
    
    void layoutLeft(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutRight(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutTop(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutBottom(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutCenter(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutLeftTop(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutRightTop(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutLeftBottom(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);
    void layoutRightBottom(Node2d::ptr view, Node2d::ptr target, float offsetX = 0.0, float offsetY = 0.0);

    void layoutParentLeft(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentRight(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentTop(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentBottom(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentCenter(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentLeftTop(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentRightTop(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentLeftBottom(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    void layoutParentRightBottom(Node2d::ptr view, float offsetX = 0.0, float offsetY = 0.0);
    
    void layout(Node2d::ptr view, AlignType from, Node2d::ptr target, AlignType to, float offsetx = 0.0, float offsetY = 0.0);
    void layoutParent(Node2d::ptr view, AlignType from, AlignType to, float offsetx = 0.0, float offsetY = 0.0);
    
    void layout(Node2d::ptr view, float viewAlignX, float viewAlignY, Node2d::ptr target,
                float targetAlignX, float targetAlignY, bool targetIsParent = false, float offsetX = 0.0, float offsetY = 0.0);
    
}

}

#endif /* defined(__LayoutUtil__) */
