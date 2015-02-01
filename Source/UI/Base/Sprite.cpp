#include "Sprite.h"

#include "HierarchyColor.h"
#include "Transform2D.h"

#include "Scene/Mesh.h"
#include "UI/Base/QuadStuffer.h"
#include "Texture/Frame/TextureFrameManager.h"
#include "Render/BufferObject/BufferObjectUtil.h"

namespace re {

void Sprite::init(const std::string& tex)
{
    TextureFrame::ptr texture = TextureFrameManager::getInstance().getFrame(tex);
    assert(texture != nullptr);

    Rect rect;
    rect.size = texture->getOriginalSize();

    this->init(texture, rect);
}

void Sprite::init(const string &tex, const Rect &rect)
{
    TextureFrame::ptr texture = TextureFrameManager::getInstance().getFrame(tex);
    assert(texture != nullptr);

    this->init(texture, rect);
}

void Sprite::init(const TextureFrame::ptr &tex, const Rect &rect)
{
    this->frame = tex;
    this->rect = rect;

    this->getComponent<Transform2D>()->setSize(rect.size);

    auto node = this->getNode();
    InitNodeForLeaf(node, frame->getTexture(), "Shader_PTC");

    this->rebind();
}

void Sprite::rebind()
{
    auto mesh = this->getComponent<Mesh>();
    auto color = this->getComponent<HierarchyColor>();

    QuadStuffer::FillQuad(frame, rect.size, color->getDisplayColor(), mesh->getGeometry());
    BufferObjectUtil::getInstance().loadGeometryToHardware(*(mesh.get()));
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

    const Sprite* inst = dynamic_cast<const Sprite*>(component);
    if (inst) {
        this->frame = inst->frame;
        this->rect = inst->rect;
    }
}

}
