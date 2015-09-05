#include "OpenALAudioEngine.h"

#include "OpenALBase.h"
#include <AL/alc.h>
#include "OpenALListener.h"
#include "OpenALSource.h"

namespace re {

OpenALAudioEngine::OpenALAudioEngine()
{
    ALCdevice* device = alcOpenDevice(NULL);
    ALCcontext* context = alcCreateContext(device, NULL);

    ALCenum alcErr = alcGetError(device);
    if (!context || alcErr != ALC_NO_ERROR)
    {
        alcCloseDevice(device);
        LogError("Unable to create OpenAL context. Error: {}", alcErr);
        return;
    }

    alcMakeContextCurrent(context);
    alcErr = alcGetError(device);
    if (alcErr != ALC_NO_ERROR)
    {
        LogError("Unable to make OpenAL context current. Error: {}", alcErr);
    }

//    alDistanceModel(AL_INVERSE_DISTANCE);

//    alDopplerFactor(1);
//    alDopplerVelocity(343);
}

OpenALAudioEngine::~OpenALAudioEngine()
{
    ALCcontext* context(alcGetCurrentContext());
    ALCdevice* device(alcGetContextsDevice(context));

    alcMakeContextCurrent(0);

    alcDestroyContext(context);
    alcCloseDevice(device);
}

SharedPtr<AudioListenerImpl> OpenALAudioEngine::CreateListenerImpl()
{
    return Create<OpenALListener>();
}

SharedPtr<AudioSourceImpl> OpenALAudioEngine::CreateSourceImpl()
{
    return Create<OpenALSource>();
}

} // namespace re

