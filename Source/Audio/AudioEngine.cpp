#include "AudioEngine.h"

#include "OpenAL/OpenALAudioEngine.h"

namespace re {

AudioEngine::AudioEngine()
{
    this->impl_ = Create<OpenALAudioEngine>();
}

AudioEngine::~AudioEngine()
{

}

SharedPtr<AudioListenerImpl> AudioEngine::CreateListenerImpl()
{
    return impl_->CreateListenerImpl();
}

SharedPtr<AudioSourceImpl> AudioEngine::CreateSourceImpl()
{
    return impl_->CreateSourceImpl();
}

} // namespace re

