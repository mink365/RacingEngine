/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "OpenALListener.h"

#include <AL/al.h>

namespace re {

OpenALListener::OpenALListener()
{

}

OpenALListener::~OpenALListener()
{

}

void OpenALListener::setGain(float gain)
{
    AL_CHECK( alListenerf(AL_GAIN, gain) );
}

void OpenALListener::setPosition(const Vec3 &pos)
{
    AL_CHECK( alListenerfv(AL_POSITION, pos.toFloatPtr()) );
}

void OpenALListener::setRotation(const Quat &rot)
{
    // TODO: quat to forward and up vector
}

void OpenALListener::setVelocity(const Vec3 &vel)
{
    AL_CHECK( alListenerfv(AL_VELOCITY, vel.toFloatPtr()) );
}

} // namespace re

