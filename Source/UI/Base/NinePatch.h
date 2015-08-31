#ifndef RE_UI_NINEPATCH_H
#define RE_UI_NINEPATCH_H

#include "Graphic.h"
#include "Math/Rect.h"
#include "Texture/Frame/TextureFrame.h"
#include "UI/Layout/LayoutUtil.h"
#include "PreDeclare.h"

namespace re {
namespace ui {

class NineGrid {
public:
    Rect lb;
    Rect rt;

    const Rect getRect(AlignType type) const;
};

class NinePatch : public Graphic
{
public:
    NinePatch()
        : Graphic()
    {};

    void init(const std::string& tex);

    void setStretch(float x, float y, float w, float h);
    void setStreth(float w, float h);
    void setStrethPadding(float leftPadding, float topPadding, float rightPadding, float bottomPadding);

    float getLeftPadding() const;
    float getRightPadding() const;
    float getBottomPadding() const;
    float getTopPadding() const;

    void rebind();

protected:
    void addQuad(AlignType type);

    virtual void updateViewColor();

protected:
    void copyProperties(const NinePatch& rhs);

protected:
    Rect centerRect;

    TextureFrame::ptr frame;

    NineGrid vertexGrid;
    NineGrid textureGrid;

private:
    HierarchyColorPtr color;
    Transform2DPtr transform;
};

} // namespace ui
} // namespace re

#endif // RE_UI_NINEPATCH_H
