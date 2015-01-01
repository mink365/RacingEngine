/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "AudioSource.h"

namespace re {

AudioSource::AudioSource()
    :looped(false), gain(1.0f), pitch(1.0f)
{

}

AudioSource::~AudioSource()
{

}

bool AudioSource::isLooped() const
{
    return looped;
}

void AudioSource::setLooped(bool looped)
{
    this->looped = looped;
}

float AudioSource::getGain() const
{
    return gain;
}

float AudioSource::getPitch() const
{
    return pitch;
}

const Vec3 &AudioSource::getPosition() const
{
    return position;
}

const Vec3 &AudioSource::getVelocity() const
{
    return velocity;
}

void AudioSource::setGain(float gain)
{
    this->gain = gain;
}

void AudioSource::setPitch(float pitch)
{
    this->pitch = pitch;
}

void AudioSource::setPosition(const Vec3 &pos)
{
    this->position = pos;
}

void AudioSource::setVelocity(const Vec3 &vel)
{
    this->velocity = vel;
}

} // namespace re

