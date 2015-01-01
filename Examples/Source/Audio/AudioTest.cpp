/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "AudioTest.h"

#include "FileSystem/FileSystem.h"

#include "Audio/AudioSource.h"
#include "Audio/AudioListener.h"
#include "Audio/OpenAL/OpenALSource.h"
#include "Audio/OpenAL/OpenALListener.h"
#include "Audio/OpenAL/OpenALBuffer.h"

#include <AL/alc.h>

AudioTest::AudioTest()
{
    this->name = "AudioTest";
}

void InitOpenAL()
{
    auto _alcDevice = alcOpenDevice(NULL);
    if (!_alcDevice)
    {
        LOG_E("Unable to open OpenAL device.\n");
        return;
    }

    auto _alcContext = alcCreateContext(_alcDevice, NULL);
    ALCenum alcErr = alcGetError(_alcDevice);
    if (!_alcContext || alcErr != ALC_NO_ERROR)
    {
        alcCloseDevice(_alcDevice);
        LOG_E("Unable to create OpenAL context. Error: %d\n", alcErr);
        return;
    }

    alcMakeContextCurrent(_alcContext);
    alcErr = alcGetError(_alcDevice);
    if (alcErr != ALC_NO_ERROR)
    {
        LOG_E("Unable to make OpenAL context current. Error: %d\n", alcErr);
    }
}

std::shared_ptr<AudioSource> CreateSource(const string& path)
{
    ALuint alBuffer;
    alGenBuffers(1, &alBuffer);

    auto file = FileSystem::getInstance().getFile(path);

    RE_ASSERT(file != nullptr);

    file->open();
    OpenALBuffer::loadOgg(file.get(), alBuffer);

    OpenALBuffer::ptr buffer = std::make_shared<OpenALBuffer>(path, alBuffer);

    // Load the audio source.
    ALuint alSource = 0;

    AL_CHECK( alGenSources(1, &alSource) );
    if (AL_LAST_ERROR())
    {
        LOG_E("Error generating audio source.");
        return NULL;
    }

    return std::make_shared<OpenALSource>(buffer, alSource);
}

void AudioTest::Init()
{
    InitOpenAL();

    source = CreateSource("Sound/Bomb.ogg");

    listener = std::make_shared<OpenALListener>();

}

float t = 0;
Vec3 pos;

void AudioTest::Update(float dt)
{
    t += dt;

    if (t > 3) {
        source->play();

        t = 0;
    }

    pos += Vec3(-1, 0, 0) * dt * 3;
    listener->setPosition(pos);
}

