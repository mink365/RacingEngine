#include "Sprite.h"
#include "HierarchyColor.h"
#include "Transform2D.h"

#include "UI/Rendering/CanvasRenderElement.h"
#include "UI/Base/QuadStuffer.h"
#include "PreDeclare.h"

namespace re {
namespace ui {

void Sprite::init()
{
    auto element = this->getEntity()->addComponent<CanvasRenderElement>();

    element->setGeometry(geometry);
}

void Sprite::init(const std::string& tex)
{
    auto frame = TextureFrameManager::instance().GetResource(tex);

    this->setFrame(frame);
}

void Sprite::setFrame(TextureFrame::ptr frame)
{
    this->frame = frame;
    this->rect.size = frame->getOriginalSize();

    auto element = this->getEntity()->addComponent<CanvasRenderElement>();
    auto material = CreateDefaultMaterial(frame->getTexture(), "Shader_PTC");
    element->setMaterial(material);
    element->setTexture(frame->getTexture());

    this->rebind();
}

void Sprite::rebind()
{
    auto color = this->getComponent<HierarchyColor>();

    QuadStuffer::FillQuad(frame, rect.size, color->getDisplayColor(), geometry);
}

void Sprite::updateViewColor()
{
    this->rebind();
}

void Sprite::copyProperties(const Sprite& rhs)
{

    this->frame = rhs.frame;
    this->rect = rhs.rect;
}

} // namespace ui
} // namespace re
