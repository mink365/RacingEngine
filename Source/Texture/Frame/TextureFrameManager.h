#ifndef TEXTUREFRAMEMANAGER_H
#define TEXTUREFRAMEMANAGER_H

#include <map>
#include "Base/Singleton.h"

#include "TextureFrame.h"

namespace re {

class TextureFrameManager : public Singleton<TextureFrameManager>
{
    friend class Singleton;

public:
    void registerFrame(TextureFrame::ptr& frame);
    void desposeFrame(TextureFrame::constPtr& frame);

    bool containFrame(std::string name);
    TextureFrame::ptr getFrame(std::string name);

private:
    TextureFrameManager();

private:
    std::map<std::string, TextureFrame::constPtr> frames;
};

}

#endif // TEXTUREFRAMEMANAGER_H
