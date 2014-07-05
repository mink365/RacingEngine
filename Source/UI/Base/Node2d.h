#ifndef NODE2D_H
#define NODE2D_H

#include "Math/BoundingVolume.h"
#include "Math/Color.h"
#include "Base/Shared.h"
#include "Scene/SceneNode.h"
#include "Geometry/Geometry.h"
#include "Texture/Texture.h"

namespace re {

void InitNodeForLeaf(SceneNodePtr& node, Texture::ptr texture);

class Rgba {
public:
    void setAlpha(float a);
    float getAlpha() const;

    void setColor(const Color& color);
    const Color& getColor() const;

    const Color& getDisplayColor() const;

protected:
    Color color;

    /**
     * @brief worldColor
     */
    Color worldColor;
};

class Node2d : public SceneNode, public Shared<Node2d>, public Rgba
{
public:
    Node2d();
    virtual bool init() {return true;};

    Node2d::ptr getParent() const;
    void removeFromParent();

    virtual void onEnter();
    virtual void onExit();

    bool isRunning();
    bool isVisible();

    void stopAllActions();

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

    void setAnchorPoint(const Vec2& v);
    const Vec2& getAnchorPoint() const;
    Vec2 getAnchorPointInPixels() const;

    void setContentSize(const Size& size);
    const Size &getContentSize() const;

    Rect getBoundingBox() const;

    /**
     * Converts a Vec2 to node (local) space coordinates. The result is in Points.
     */
    Vec2 convertToNodeSpace(const Vec2& worldPoint) const;

    /**
     * Converts a Vec2 to world space coordinates. The result is in Points.
     */
    Vec2 convertToWorldSpace(const Vec2& nodePoint) const;

protected:
    Geometry::ptr getGeometry() const;

protected:
    Size size;
    Vec2 anchorPoint;
};

}

#endif // NODE2D_H
