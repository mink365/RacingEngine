#include "TextureFrameManager.h"

namespace re {

void TextureFrameManager::registerFrame(TextureFrame::ptr &frame)
{
    this->frames[frame->getName()] = frame;
}

void TextureFrameManager::desposeFrame(TextureFrame::constPtr &frame)
{
    if (this->frames.count(frame->getName())) {
        this->frames.erase(frame->getName());
    }
}

bool TextureFrameManager::containFrame(string name)
{
    return this->frames.count(name);
}

TextureFrame::ptr TextureFrameManager::getFrame(string name)
{
    if (this->containFrame(name)) {
        return this->frames[name];
    }

    return nullptr;
}

TextureFrameManager::TextureFrameManager()
{
}

}
