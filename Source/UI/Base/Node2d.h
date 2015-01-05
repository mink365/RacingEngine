#ifndef NODE2D_H
#define NODE2D_H

#include "Math/Rect.h"
#include "Math/Color.h"
#include "Base/Shared.h"
#include "Scene/SceneNode.h"
#include "Geometry/Geometry.h"
#include "Texture/Texture.h"
#include "Transform2D.h"

namespace re {

template<class T, typename... Args>
std::shared_ptr<T> CreateView(Args... args) {
    auto view = std::make_shared<T>();

    if (view != nullptr) {
        view->init(args...);
    }

    return view;
}

class Transform2D;
typedef std::shared_ptr<Transform2D> Transform2DPtr;

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
    friend class Transform2D;

public:
    Node2d();

    Node2d::ptr getParent() const;
    void removeFromParent();

    virtual void onEnter();
    virtual void onExit();

    bool isRunning();
    bool isVisible();

    void stopAllActions();

    void setAnchorPoint(const Vec2& v);
    const Vec2& getAnchorPoint() const;
    Vec2 getAnchorPointInPixels() const;

    void setContentSize(const Size& size);
    const Size &getContentSize() const;

    Rect getBoundingBox() const;

    Transform2DPtr getTransform2D();

public:
    virtual void addChild(NodePtr node, Int index=-1) override;
    virtual void removeChild(NodePtr node) override;
    virtual void removeAllChildren() override;

protected:
    Geometry::ptr getGeometry();
    MeshPtr getMesh();

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
