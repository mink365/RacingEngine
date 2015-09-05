/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "AudioListener.h"
#include "AudioEngine.h"

namespace re {

AudioListener::AudioListener()
    : gain(1.0f)
{

}

AudioListener::~AudioListener()
{

}

float AudioListener::getGain() const
{
    return gain;
}

const Vec3 &AudioListener::getPosition() const
{
    return position;
}

const Quat &AudioListener::getRotation() const
{
    return rotation;
}

const Vec3 &AudioListener::getVelocity() const
{
    return velocity;
}

void AudioListener::setGain(float gain)
{
    this->gain = gain;

    impl->setGain(gain);
}

void AudioListener::setPosition(const Vec3 &pos)
{
    this->position = pos;

    impl->setPosition(pos);
}

void AudioListener::setRotation(const Quat &rot)
{
    this->rotation = rot;

    impl->setRotation(rot);
}

void AudioListener::setVelocity(const Vec3 &vel)
{
    this->velocity = vel;

    impl->setVelocity(vel);
}

void AudioListener::registerEvents()
{
    RegisterEvent(Events::Awake, this, &AudioListener::onAwake);
}

void AudioListener::onAwake()
{
    this->impl = AudioEngine::instance().CreateListenerImpl();
    this->impl->component = this->getComponent<AudioListener>();
}

} // namespace re

