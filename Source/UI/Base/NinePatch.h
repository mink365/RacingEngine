#ifndef NINEPATCH_H
#define NINEPATCH_H

#include "Component.h"
#include "Math/Rect.h"
#include "Texture/Frame/TextureFrame.h"
#include "UI/Layout/LayoutUtil.h"

namespace re {

class NineGrid {
public:
    Rect lb;
    Rect rt;

    const Rect getRect(AlignType type) const;
};

class NinePatch : public Component
{
public:
    NinePatch() {};

    void init(const std::string& tex);

    void setStretch(float x, float y, float w, float h);
    void setStreth(float w, float h);
    void setStrethPadding(float leftPadding, float topPadding, float rightPadding, float bottomPadding);

    float getLeftPadding() const;
    float getRightPadding() const;
    float getBottomPadding() const;
    float getTopPadding() const;

    virtual void rebind();

protected:
    void addQuad(AlignType type);

    virtual void updateViewColor();

protected:
    virtual ComponentPtr createCloneInstance() const;
    virtual void copyProperties(const Component* component) override;

protected:
    Rect centerRect;

    TextureFrame::ptr frame;

    NineGrid vertexGrid;
    NineGrid textureGrid;

private:
    MeshPtr mesh;
    HierarchyColorPtr color;
    Transform2DPtr transform;
};

}

#endif // NINEPATCH_H
