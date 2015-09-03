#include "Image.h"
#include "HierarchyColor.h"
#include "Transform2D.h"

#include "UI/Rendering/CanvasRenderElement.h"
#include "UI/Base/QuadStuffer.h"
#include "PreDeclare.h"

namespace re {
namespace ui {

void Image::init()
{
    auto element = this->getEntity()->addComponent<CanvasRenderElement>();

    element->setGeometry(geometry);
}

void Image::init(const std::string& tex)
{
    auto frame = TextureFrameManager::instance().GetResource(tex);

    this->setFrame(frame);
}

void Image::setFrame(TextureFrame::ptr frame)
{
    this->frame = frame;
    this->rect.size = frame->getOriginalSize();

    auto element = this->getEntity()->addComponent<CanvasRenderElement>();
    auto material = CreateDefaultMaterial(frame->getTexture(), "Shader_PTC");
    element->setMaterial(material);
    element->setTexture(frame->getTexture());

    this->rebind();
}

void Image::rebind()
{
    auto color = this->getComponent<HierarchyColor>();

    QuadStuffer::FillQuad(frame, rect.size, color->getDisplayColor(), geometry);
}

void Image::updateViewColor()
{
    this->rebind();
}

void Image::copyProperties(const Image& rhs)
{

    this->frame = rhs.frame;
    this->rect = rhs.rect;
}

} // namespace ui
} // namespace re
