#include "Sprite.h"

#include "HierarchyColor.h"
#include "Transform2D.h"

#include "UI/Rendering/CanvasRenderElement.h"
#include "Scene/Mesh.h"
#include "UI/Base/QuadStuffer.h"
#include "Render/BufferObject/BufferObjectUtil.h"

namespace re {
namespace ui {

void Sprite::init(const std::string& tex)
{
    TextureFrame::ptr texture = TextureFrameManager::instance().GetResource(tex);
    assert(texture != nullptr);

    Rect rect;
    rect.size = texture->getOriginalSize();

    this->init(texture, rect);
}

void Sprite::init(const string &tex, const Rect &rect)
{
    TextureFrame::ptr texture = TextureFrameManager::instance().GetResource(tex);
    assert(texture != nullptr);

    this->init(texture, rect);
}

void Sprite::init(const TextureFrame::ptr &tex, const Rect &rect)
{

    this->frame = tex;
    this->rect = rect;

    this->getComponent<Transform2D>()->setSize(rect.size);

    auto element = this->getEntity()->addComponent<CanvasRenderElement>();

    auto material = CreateDefaultMaterial(tex->getTexture(), "Shader_PTC");
    element->setMaterial(material);
    element->setTexture(tex->getTexture());
    element->setGeometry(geometry);

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

ComponentPtr Sprite::createCloneInstance() const
{
    return CreateCloneInstance<Sprite>();
}

void Sprite::copyProperties(const Component *component)
{
    Component::copyProperties(component);

    const Sprite* inst = static_cast<const Sprite*>(component);
    if (inst) {
        this->frame = inst->frame;
        this->rect = inst->rect;
    }
}

} // namespace ui
} // namespace re
