/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_UI_TRANSFORM2D_H
#define RE_UI_TRANSFORM2D_H

#include "Math/Size.h"
#include "Math/Rect.h"
#include "Scene/Transform.h"

namespace re {
namespace ui {

class Transform2D;
typedef ComponentHandle<Transform2D> Transform2DPtr;

class Transform2D : public Transform
{
public:
    Transform2D();
    ~Transform2D();

    void setPosition(const Vec2& position);
    Vec2 getPosition() const;

    void setScale(const Vec2& scale);
    Vec2 getScale() const;
    void setScale(float v);

    void setRotation(float v);
    float getRotation() const;

    void setAnchor(const Vec2& v);
    const Vec2& getAnchor() const;
    void setAnchorInPixel(const Vec2& v);
    Vec2 getAnchorInPixel() const;

    void setSize(const Size& size);
    const Size &getSize() const;
    void setWidth(float v);
    void setHeight(float v);

    Rect getBoundingBox() const;

    /**
     * Converts a Vec2 to node (local) space coordinates. The result is in Points.
     */
    Vec2 convertToNodeSpace(const Vec2& worldPoint) const;

    /**
     * Converts a Vec2 to world space coordinates. The result is in Points.
     */
    Vec2 convertToWorldSpace(const Vec2& nodePoint) const;

    Vec2 convertParentToLocalSpace(const Vec2& point) const;
    Vec2 convertNodeToParentSpace(const Vec2& point) const;

protected:
    virtual void updateLocalMatrix();

    void copyProperties(const Transform2D& rhs);

private:
    Size size;

    Vec2 anchor;
    Vec2 anchorInPixel;

    Vec2 skew;
};

} // namespace ui
} // namespace re

#endif // RE_UI_TRANSFORM2D_H
