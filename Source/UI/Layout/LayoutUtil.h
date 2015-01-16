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

namespace LayoutUtil {
    Vec2 AlignTypeToPoint(AlignType type);
    
    Vec2 GetBoundBoxPosition(Transform2DPtr node, AlignType type);
    
    void LayoutLeft(Transform2DPtr view, Transform2DPtr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutRight(Transform2DPtr view, Transform2DPtr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutTop(Transform2DPtr view, Transform2DPtr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutBottom(Transform2DPtr view, Transform2DPtr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutCenter(Transform2DPtr view, Transform2DPtr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutLeftTop(Transform2DPtr view, Transform2DPtr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutRightTop(Transform2DPtr view, Transform2DPtr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutLeftBottom(Transform2DPtr view, Transform2DPtr target, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutRightBottom(Transform2DPtr view, Transform2DPtr target, float offsetX = 0.0, float offsetY = 0.0);

    void LayoutToParentLeft(Transform2DPtr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentRight(Transform2DPtr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentTop(Transform2DPtr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentBottom(Transform2DPtr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentCenter(Transform2DPtr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentLeftTop(Transform2DPtr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentRightTop(Transform2DPtr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentLeftBottom(Transform2DPtr view, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParentRightBottom(Transform2DPtr view, float offsetX = 0.0, float offsetY = 0.0);
    
    void Layout(Transform2DPtr view, AlignType from, Transform2DPtr target, AlignType to, float offsetx = 0.0, float offsetY = 0.0);
    void LayoutToParent(Transform2DPtr view, AlignType from, AlignType to, float offsetx = 0.0, float offsetY = 0.0);
    
    void Layout(Transform2DPtr view, float viewAlignX, float viewAlignY, Transform2DPtr target,
                float targetAlignX, float targetAlignY, bool targetIsParent = false, float offsetX = 0.0, float offsetY = 0.0);
    void LayoutToParent(Transform2DPtr view, float viewAlignX, float viewAlignY, float targetAlignX, float targetAlignY,float offsetX = 0.0, float offsetY = 0.0);
    
}

}

#endif /* defined(__LayoutUtil__) */
