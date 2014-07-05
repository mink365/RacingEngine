#include "Sprite.h"

#include "Texture/Frame/TextureFrameManager.h"
#include "UI/Base/QuadStuffer.h"

namespace re {

Sprite::Sprite(const std::string& tex)
{
    TextureFrame::ptr texture = TextureFrameManager::getInstance().getFrame(tex);
    assert(this->texture);

    Rect rect;
    rect.size = this->texture->getOriginalSize();

    this->init(texture, rect);
}

Sprite::Sprite(const string &tex, const Rect &rect)
{
    TextureFrame::ptr texture = TextureFrameManager::getInstance().getFrame(tex);
    assert(this->texture);

    this->init(texture, rect);
}

void Sprite::init(const TextureFrame::ptr &tex, const Rect &rect)
{
    this->texture = tex;
    this->rect = rect;

    QuadStuffer::FillQuad(tex, rect.size, this->getGeometry());
}

}
