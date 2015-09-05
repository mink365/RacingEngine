#ifndef RE_OPENALAUDIOENGINE_H
#define RE_OPENALAUDIOENGINE_H

#include "Audio/AudioEngine.h"
#include "Base/Singleton.h"

namespace re {

class OpenALAudioEngine : public AudioEngineImpl
{
public:
    OpenALAudioEngine();
    ~OpenALAudioEngine();

    SharedPtr<AudioListenerImpl> CreateListenerImpl() override;
    SharedPtr<AudioSourceImpl> CreateSourceImpl() override;
};

} // namespace re

#endif // RE_OPENALAUDIOENGINE_H
