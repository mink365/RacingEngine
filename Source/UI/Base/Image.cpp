#include "Image.h"
#include "HierarchyColor.h"
#include "Transform2D.h"

#include "UI/Rendering/CanvasRenderElement.h"
#include "UI/Base/QuadStuffer.h"
#include "PreDeclare.h"

namespace re {
namespace ui {

COMPONENT_DEPENDENCY(Image, CanvasRenderElement);

void Image::setFrame(const std::string& tex)
{
    auto frame = TextureFrameManager::instance().GetResource(tex);

    this->setFrame(frame);
}

void Image::setFrame(TextureFrame::ptr frame)
{
    this->frame = frame;
    this->rect.size = frame->getOriginalSize();

    auto element = this->getComponent<CanvasRenderElement>();
    // TODO: avoide create the material
    auto material = CreateDefaultMaterial(frame->getTexture(), "Shader_PTC");
    element->setMaterial(material);
    element->setTexture(frame->getTexture());

    this->rebind();
}

void Image::onAwake()
{
    this->frame = TextureFrameManager::instance().GetResource("store_icon_coin.png");
    this->rect.size = frame->getOriginalSize();

    auto element = this->getComponent<CanvasRenderElement>();
    auto material = CreateDefaultMaterial(frame->getTexture(), "Shader_PTC");
    element->setMaterial(material);
    element->setTexture(frame->getTexture());

    this->rebind();
}

void Image::registerEvents()
{
    Graphic::registerEvents();
    RegisterEvent(Events::Awake, this, &Image::onAwake);
}

void Image::addQuad(AlignType type, const NineGrid &vertexGrid, const NineGrid &textureGrid, const Color& color)
{
    Rect vRect = vertexGrid.getRect(type);
    Rect tRect = textureGrid.getRect(type);

    QuadStuffer::AddOriginalQuad(vRect, tRect, color, frame, geometry);
}

void Image::rebind()
{
    auto color = this->getComponent<HierarchyColor>();
    auto transform = this->getComponent<Transform2D>();

    if (type == ImageType::Sample)
    {
        QuadStuffer::FillQuad(frame, rect.size, color->getDisplayColor(), geometry);
    }
    else if (type == ImageType::NinePatch)
    {
        auto& data = getData<ImageType::NinePatch>();

        float x = data.leftPadding;
        float y = data.bottomPadding;
        float w = (this->frame->getOriginalSize().width - data.leftPadding - data.rightPadding);
        float h = (this->frame->getOriginalSize().height - data.topPadding - data.bottomPadding);
        Rect centerRect = {x, y, w, h};

        NineGrid vertexGrid, textureGrid;

        vertexGrid.lb.set(0, 0, centerRect.origin.x, centerRect.origin.y);
        textureGrid.lb.set(0, 0, centerRect.origin.x, centerRect.origin.y);

        vertexGrid.rt.set(transform->getSize().width - data.rightPadding,
                                transform->getSize().height - data.topPadding,
                                data.rightPadding,
                                data.topPadding);

        textureGrid.rt.set(centerRect.getMaxX(), centerRect.getMaxY(),
                            data.rightPadding, data.topPadding);

        geometry->clear();

        Color _color = color->getDisplayColor();
        this->addQuad(AlignType::LEFT_BOTTOM, vertexGrid, textureGrid, _color);
        this->addQuad(AlignType::LEFT_CENTER, vertexGrid, textureGrid, _color);
        this->addQuad(AlignType::LEFT_TOP, vertexGrid, textureGrid, _color);
        this->addQuad(AlignType::CENTER_TOP, vertexGrid, textureGrid, _color);
        this->addQuad(AlignType::RIGHT_TOP, vertexGrid, textureGrid, _color);
        this->addQuad(AlignType::RIGHT_CENTER, vertexGrid, textureGrid, _color);
        this->addQuad(AlignType::RIGHT_BOTTOM, vertexGrid, textureGrid, _color);
        this->addQuad(AlignType::CENTER_BOTTOM, vertexGrid, textureGrid, _color);
        this->addQuad(AlignType::CENTER, vertexGrid, textureGrid, _color);
    }
}

void Image::updateViewColor()
{
    auto color = this->getComponent<HierarchyColor>();
    auto& colors = geometry->getDiffuseColors();
    for (size_t i = 0; i < colors.size(); ++i) {
        colors[i] = color->getDisplayColor();
    }
}

void Image::copyProperties(const Image& rhs)
{
    this->frame = rhs.frame;
    this->rect = rhs.rect;
    this->datas = rhs.datas;
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
