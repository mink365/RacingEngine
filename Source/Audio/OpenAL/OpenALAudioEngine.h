#ifndef RE_OPENALAUDIOENGINE_H
#define RE_OPENALAUDIOENGINE_H

#include "Audio/AudioEngine.h"
#include "Base/Singleton.h"

namespace re {

class OpenALAudioEngine : public AudioEngine, public Singleton<OpenALAudioEngine>
{
public:
    OpenALAudioEngine();
    ~OpenALAudioEngine();
};

} // namespace re

#endif // RE_OPENALAUDIOENGINE_H
