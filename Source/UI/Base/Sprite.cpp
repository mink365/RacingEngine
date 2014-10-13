#include "Sprite.h"

#include "Texture/Frame/TextureFrameManager.h"
#include "UI/Base/QuadStuffer.h"
#include "Render/BufferObject/BufferObjectUtil.h"

namespace re {

Sprite::Sprite(const std::string& tex)
{
    TextureFrame::ptr texture = TextureFrameManager::getInstance().getFrame(tex);
//    assert(this->texture != nullptr);

    Rect rect;
    rect.size = texture->getOriginalSize();

    this->init(texture, rect);
}

Sprite::Sprite(const string &tex, const Rect &rect)
{
    TextureFrame::ptr texture = TextureFrameManager::getInstance().getFrame(tex);
    assert(this->frame != nullptr);

    this->init(texture, rect);
}

void Sprite::rebind()
{
    SceneNodePtr node = std::dynamic_pointer_cast<SceneNode>(this->shared_from_this());
    InitNodeForLeaf(node, frame->getTexture(), "Shader_PTC");

    QuadStuffer::FillQuad(frame, rect.size, this->worldColor, this->getGeometry());

    BufferObjectUtil::getInstance().loadGeometryToHardware(*(this->getMesh().get()));
}

void Sprite::init(const TextureFrame::ptr &tex, const Rect &rect)
{
    this->frame = tex;
    this->rect = rect;

    this->setContentSize(rect.size);
}

void Sprite::updateViewColor()
{
    this->rebind();
}

NodePtr Sprite::createCloneInstance() const
{
    return CreateCloneInstance<Sprite>();
}

void Sprite::copyProperties(const Node *node)
{
    Node2d::copyProperties(node);

    const Sprite* inst = dynamic_cast<const Sprite*>(node);
    if (inst) {
        this->frame = inst->frame;
        this->rect = inst->rect;
    }
}

}
