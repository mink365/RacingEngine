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

void AudioSource::play()
{
    impl->play();
}

void AudioSource::pause()
{
    impl->pause();
}

void AudioSource::resume()
{
    impl->resume();
}

void AudioSource::stop()
{
    impl->stop();
}

void AudioSource::rewind()
{
    impl->rewind();
}

bool AudioSource::isLooped() const
{
    return looped;
}

void AudioSource::setLooped(bool looped)
{
    this->looped = looped;

    impl->setLooped(looped);
}

AudioSourceState AudioSource::getState() const
{
    return impl->getState();
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

    impl->setGain(gain);
}

void AudioSource::setPitch(float pitch)
{
    this->pitch = pitch;

    impl->setPitch(pitch);
}

void AudioSource::setPosition(const Vec3 &pos)
{
    this->position = pos;

    impl->setPosition(pos);
}

void AudioSource::setVelocity(const Vec3 &vel)
{
    this->velocity = vel;

    impl->setVelocity(vel);
}

} // namespace re

