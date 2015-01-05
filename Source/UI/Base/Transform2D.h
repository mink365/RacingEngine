/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_TRANSFORM2D_H
#define RE_TRANSFORM2D_H

#include "Transform.h"

namespace re {

class Node2d;

class Transform2D : public Transform
{
public:
    Transform2D();
    ~Transform2D();

    void setPosition(const Vec2& position);
    Vec2 getPosition() const;

    void setPosition(float x, float y);
    void setPositionX(float v);
    void setPositionY(float v);
    float getPositionX() const;
    float getPositionY() const;

    void setScale(const Vec2& scale);
    Vec2 getScale() const;

    void setScale(float x, float y);
    void setScale(float v);
    void setScaleX(float v);
    void setScaleY(float v);
    float getScaleX() const;
    float getScaleY() const;

    void setRotation(float v);
    float getRotation() const;

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

    Node2d& getNode2D();

};

} // namespace re

#endif // RE_TRANSFORM2D_H
