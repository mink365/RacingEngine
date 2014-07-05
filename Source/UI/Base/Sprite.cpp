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
    InitNodeForLeaf(node, frame->getTexture());

    QuadStuffer::FillQuad(frame, rect.size, this->getGeometry());

    BufferObjectUtil::getInstance().loadGeometryToHardware(*(this->getGeometry().get()));
}

void Sprite::init(const TextureFrame::ptr &tex, const Rect &rect)
{
    this->frame = tex;
    this->rect = rect;
}

}
