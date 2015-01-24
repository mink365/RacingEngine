/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "OpenALSource.h"

namespace re {

OpenALSource::OpenALSource()
    : _alSource(0), _buffer(nullptr)
{
    AL_CHECK( alGenSources(1, &_alSource) );
    if (AL_LAST_ERROR())
    {
        LOG_E("Error generating audio source.");
    }
}

OpenALSource::~OpenALSource()
{
    LOG_E("remove source");

    if (_alSource)
    {
        AL_CHECK( alDeleteSources(1, &_alSource) );
        _alSource = 0;
    }
}

void OpenALSource::bindBuffer(OpenALBuffer::ptr &buffer)
{
    RE_ASSERT(buffer);
    _buffer = buffer;

    AL_CHECK( alSourcei(_alSource, AL_BUFFER, _buffer->_alBuffer) );
    AL_CHECK( alSourcei(_alSource, AL_LOOPING, looped) );
    AL_CHECK( alSourcef(_alSource, AL_PITCH, pitch) );
    AL_CHECK( alSourcef(_alSource, AL_GAIN, gain) );
    AL_CHECK( alSourcefv(_alSource, AL_VELOCITY, velocity.toFloatPtr()) );
}

void OpenALSource::play()
{
    AL_CHECK( alSourcePlay(_alSource) );
}

void OpenALSource::pause()
{
    AL_CHECK( alSourcePause(_alSource) );
}

void OpenALSource::resume()
{
    if (getState() == AudioSourceState::PAUSED)
    {
        play();
    }
}

void OpenALSource::stop()
{
    AL_CHECK( alSourceStop(_alSource) );
}

void OpenALSource::rewind()
{
    AL_CHECK( alSourceRewind(_alSource) );
}

void OpenALSource::setLooped(bool looped)
{
    AL_CHECK( alSourcei(_alSource, AL_LOOPING, (looped) ? AL_TRUE : AL_FALSE) );
    if (AL_LAST_ERROR())
    {
        LOG_E("Failed to set audio source's looped attribute with error: %d", AL_LAST_ERROR());
    }
    AudioSource::setLooped(looped);
}

AudioSourceState OpenALSource::getState() const
{
    ALint state;
    AL_CHECK( alGetSourcei(_alSource, AL_SOURCE_STATE, &state) );

    switch (state)
    {
        case AL_PLAYING:
            return AudioSourceState::PLAYING;
        case AL_PAUSED:
            return AudioSourceState::PAUSED;
        case AL_STOPPED:
            return AudioSourceState::STOPPED;
        default:
            return AudioSourceState::INITIAL;
    }
    return AudioSourceState::INITIAL;
}

void OpenALSource::setGain(float gain)
{
    AL_CHECK( alSourcef(_alSource, AL_GAIN, gain) );
    AudioSource::setGain(gain);
}

void OpenALSource::setPitch(float pitch)
{
    AL_CHECK( alSourcef(_alSource, AL_PITCH, pitch) );
    AudioSource::setPitch(pitch);
}

void OpenALSource::setPosition(const Vec3 &pos)
{
    AudioSource::setPosition(pos);
    AL_CHECK( alSourcefv(_alSource, AL_POSITION, position.toFloatPtr()) );
}

void OpenALSource::setVelocity(const Vec3 &vel)
{
    AudioSource::setVelocity(vel);
    AL_CHECK( alSourcefv(_alSource, AL_VELOCITY, velocity.toFloatPtr()) );
}

} // namespace re

