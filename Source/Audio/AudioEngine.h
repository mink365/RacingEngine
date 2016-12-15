#ifndef RE_AUDIOENGINE_H
#define RE_AUDIOENGINE_H

#include "Base/Shared.h"
#include "Base/Singleton.h"

namespace re {

class AudioEngineImpl;
class AudioSourceImpl;
class AudioListenerImpl;

class AudioEngine : public Singleton<AudioEngine>
{
public:
    AudioEngine();
    ~AudioEngine();

    void Pause();
    void Resume();

    SharedPtr<AudioListenerImpl> CreateListenerImpl();
    SharedPtr<AudioSourceImpl> CreateSourceImpl();

private:
    SharedPtr<AudioEngineImpl> impl_;
};

class AudioEngineImpl
{
public:
    virtual ~AudioEngineImpl() {}
    virtual SharedPtr<AudioListenerImpl> CreateListenerImpl() = 0;
    virtual SharedPtr<AudioSourceImpl> CreateSourceImpl() = 0;
};

} // namespace re

#endif // RE_AUDIOENGINE_H
