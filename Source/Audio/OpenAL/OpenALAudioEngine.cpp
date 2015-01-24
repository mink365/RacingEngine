#include "OpenALAudioEngine.h"

#include "OpenALBase.h"
#include <AL/alc.h>

namespace re {

OpenALAudioEngine::OpenALAudioEngine()
{
    ALCdevice* device = alcOpenDevice(NULL);
    ALCcontext* context = alcCreateContext(device, NULL);

    ALCenum alcErr = alcGetError(device);
    if (!context || alcErr != ALC_NO_ERROR)
    {
        alcCloseDevice(device);
        LOG_E("Unable to create OpenAL context. Error: %d\n", alcErr);
        return;
    }

    alcMakeContextCurrent(context);
    alcErr = alcGetError(device);
    if (alcErr != ALC_NO_ERROR)
    {
        LOG_E("Unable to make OpenAL context current. Error: %d\n", alcErr);
    }

//    alDistanceModel(AL_INVERSE_DISTANCE);

//    alDopplerFactor(1);
//    alDopplerVelocity(343);
}

OpenALAudioEngine::~OpenALAudioEngine()
{
    LOG_E("remove engine");

    ALCcontext* context(alcGetCurrentContext());
    ALCdevice* device(alcGetContextsDevice(context));

    alcMakeContextCurrent(0);

    alcDestroyContext(context);
    alcCloseDevice(device);
}

} // namespace re

