/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_TRANSFORM2D_H
#define RE_TRANSFORM2D_H

#include "Math/Size.h"
#include "Math/Rect.h"
#include "Scene/Transform.h"

namespace re {

class Transform2D;
typedef std::shared_ptr<Transform2D> Transform2DPtr;

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

    void setAnchorPoint(const Vec2& v);
    const Vec2& getAnchorPoint() const;
    void setAnchorPointInPixels(const Vec2& v);
    Vec2 getAnchorPointInPixels() const;

    void setSize(const Size& size);
    const Size &getSize() const;

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

    ComponentPtr createCloneInstance() const;
    void copyProperties(const Component* component);

private:
    Size size;

    Vec2 anchorPoint;
    Vec2 anchorPointInPoints;

    Vec2 skew;

};

} // namespace re

#endif // RE_TRANSFORM2D_H
