#include "NinePatch.h"

#include "HierarchyColor.h"
#include "Base/ECS/Component.h"
#include "QuadStuffer.h"

namespace re {
namespace ui {

void NinePatch::init(const std::string& tex)
{
    color = this->getComponent<HierarchyColor>();
    transform = this->getComponent<Transform2D>();

    this->frame = TextureFrameManager::instance().GetResource(tex);

    this->getEntity()->addComponent<CanvasRenderElement>();
    auto element = this->getComponent<CanvasRenderElement>();

    auto material = CreateDefaultMaterial(frame->getTexture(), "Shader_PTC");
    element->setMaterial(material);
    element->setTexture(frame->getTexture());
    element->setGeometry(geometry);

    this->rebind();
}

void NinePatch::setStretch(float x, float y, float w, float h)
{
    this->centerRect = {x, y, w, h};
}

void NinePatch::setStreth(float w, float h)
{
    float x = (this->frame->getOriginalSize().width - w) / 2.0f;
    float y = (this->frame->getOriginalSize().height - h) / 2.0f;

    this->centerRect = {x, y, w, h};
}

void NinePatch::setStrethPadding(float leftPadding, float topPadding, float rightPadding, float bottomPadding)
{
    float x = leftPadding;
    float y = bottomPadding;
    float w = (this->frame->getOriginalSize().width - leftPadding - rightPadding);
    float h = (this->frame->getOriginalSize().height - topPadding - bottomPadding);

    this->centerRect = {x, y, w, h};
}

float NinePatch::getLeftPadding() const
{
    return this->centerRect.getMinX();
}

float NinePatch::getRightPadding() const
{
    return this->frame->getOriginalSize().width - this->centerRect.getMaxX();
}

float NinePatch::getBottomPadding() const
{
    return this->centerRect.getMinY();
}

float NinePatch::getTopPadding() const
{
    return this->frame->getOriginalSize().height - this->centerRect.getMaxY();
}

/**
 * 0-----2     4-----6
 * |     |     |     |
 * |     |     |  2  |
 * 1-----3     5-----7
 *
 *
 * 8-----10    12----14
 * |     |     |     |
 * |  1  |     |     |
 * 9-----11    13----15
 */

void NinePatch::rebind()
{
    this->vertexGrid.lb.set(0, 0, this->centerRect.origin.x, this->centerRect.origin.y);
    this->textureGrid.lb.set(0, 0, this->centerRect.origin.x, this->centerRect.origin.y);

    this->vertexGrid.rt.set(transform->getSize().width - this->getRightPadding(),
                            transform->getSize().height - this->getTopPadding(),
                            this->getRightPadding(),
                            this->getTopPadding());

    this->textureGrid.rt.set(this->centerRect.getMaxX(), this->centerRect.getMaxY(),
                             this->getRightPadding(), this->getTopPadding());

    geometry->clear();

    this->addQuad(AlignType::LEFT_BOTTOM);
    this->addQuad(AlignType::LEFT_CENTER);
    this->addQuad(AlignType::LEFT_TOP);
    this->addQuad(AlignType::CENTER_TOP);
    this->addQuad(AlignType::RIGHT_TOP);
    this->addQuad(AlignType::RIGHT_CENTER);
    this->addQuad(AlignType::RIGHT_BOTTOM);
    this->addQuad(AlignType::CENTER_BOTTOM);
    this->addQuad(AlignType::CENTER);
}

void NinePatch::addQuad(AlignType type)
{
    Rect vRect = vertexGrid.getRect(type);
    Rect tRect = textureGrid.getRect(type);

    QuadStuffer::AddOriginalQuad(vRect, tRect, color->getDisplayColor(), this->frame, geometry);
}

void NinePatch::updateViewColor()
{
    auto& colors = geometry->getDiffuseColors();
    for (size_t i = 0; i < colors.size(); ++i) {
        colors[i] = color->getDisplayColor();
    }
}

void NinePatch::copyProperties(const NinePatch &rhs)
{
    this->frame = rhs.frame;
    this->centerRect = rhs.centerRect;
}

/**
 *
 *  3   4   5
 *
 *  2   9   6
 *
 *  1   8   7
 */
const Rect NineGrid::getRect(AlignType type) const
{
    Rect rect;

    Vec2 lbv;
    Vec2 rtv;

    switch (type) {
    case AlignType::CENTER:
    {
        lbv.set(lb.getMaxX(), lb.getMaxY());
        rtv.set(rt.getMinX(), rt.getMinY());

        break;
    }
    case AlignType::CENTER_BOTTOM:
    {
        lbv.set(lb.getMaxX(), lb.getMinY());
        rtv.set(rt.getMinX(), lb.getMaxY());

        break;
    }
    case AlignType::CENTER_TOP:
    {
        lbv.set(lb.getMaxX(), rt.getMinY());
        rtv.set(rt.getMinX(), rt.getMaxY());

        break;
    }
    case AlignType::LEFT_BOTTOM:
    {
        lbv.set(lb.getMinX(), lb.getMinY());
        rtv.set(lb.getMaxX(), lb.getMaxY());

        break;
    }
    case AlignType::LEFT_CENTER:
    {
        lbv.set(lb.getMinX(), lb.getMaxY());
        rtv.set(lb.getMaxX(), rt.getMinY());

        break;
    }
    case AlignType::LEFT_TOP:
    {
        lbv.set(lb.getMinX(), rt.getMinY());
        rtv.set(lb.getMaxX(), rt.getMaxY());

        break;
    }
    case AlignType::RIGHT_BOTTOM:
    {
        lbv.set(rt.getMinX(), lb.getMinY());
        rtv.set(rt.getMaxX(), lb.getMaxY());

        break;
    }
    case AlignType::RIGHT_CENTER:
    {
        lbv.set(rt.getMinX(), lb.getMaxY());
        rtv.set(rt.getMaxX(), rt.getMinY());

        break;
    }
    case AlignType::RIGHT_TOP:
    {
        lbv.set(rt.getMinX(), rt.getMinY());
        rtv.set(rt.getMaxX(), rt.getMaxY());

        break;
    }
    }

    rect.fromeVertex(lbv, rtv);

    return rect;
}

} // namespace ui
} // namespace re
