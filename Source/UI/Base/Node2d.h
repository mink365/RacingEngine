#ifndef NODE2D_H
#define NODE2D_H

#include "Math/Rect.h"
#include "Math/Color.h"
#include "Base/Shared.h"
#include "Scene/SceneNode.h"
#include "Geometry/Geometry.h"
#include "Texture/Texture.h"

namespace re {

template<class T, typename... Args>
std::shared_ptr<T> CreateView(Args... args) {
    auto view = std::make_shared<T>();

    if (view != nullptr) {
        view->init(args...);
    }

    return view;
}

void InitNodeForLeaf(SceneNodePtr& node, Texture::ptr texture, const string &shaderName);

class Rgba {
public:
    void setAlpha(float a);
    float getAlpha() const;

    void setColor(const Color& color);
    const Color& getColor() const;

    const Color& getDisplayColor() const;

protected:
    virtual void updateColor() = 0;

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

    Vec2 convertParentToLocalSpace(const Vec2& point) const;
    Vec2 convertNodeToParentSpace(const Vec2& point) const;

public:
    virtual void addChild(NodePtr node, Int index=-1) override;
    virtual void removeChild(NodePtr node) override;
    virtual void removeAllChildren() override;

protected:
    Geometry::ptr getGeometry() const;
    MeshPtr getMesh() const;

    virtual void updateLocalMatrix();
    virtual void updateColor();

    /**
     * @brief updateViewColor
     * update the color of vertex/geometry
     */
    virtual void updateViewColor();

protected:
    virtual NodePtr createCloneInstance() const;
    virtual void copyProperties(const Node* node) override;

protected:
    bool inScene;

    Size size;
    Vec2 anchorPoint;
    Vec2 anchorPointInPoints;

    Vec2 skew;

protected:
    static const int RF_COLOR = 0x01 << 3;
};

}

#endif // NODE2D_H
