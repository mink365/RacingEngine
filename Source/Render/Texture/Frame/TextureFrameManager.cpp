#include "TextureFrameManager.h"

namespace re {

void TextureFrameManager::registerFrame(TextureFrame::ptr &frame)
{
    if (frames.count(frame->getName()) == 0) {
        this->frames[frame->getName()] = frame;
    } else {
        LOG_E("TextureFrameManager::registerFrame: aready has key: %s", frame->getName().c_str());
    }
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
