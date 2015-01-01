/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_OPENALLISTENER_H
#define RE_OPENALLISTENER_H

#include "Audio/AudioListener.h"

namespace re {

class OpenALListener : public AudioListener
{
public:
    OpenALListener();
    ~OpenALListener();

public:
    virtual void setGain(float gain) override;
    virtual void setPosition(const Vec3& pos) override;
    virtual void setRotation(const Quat& rot) override;
    virtual void setVelocity(const Vec3& vel) override;
};

} // namespace re

#endif // RE_OPENALLISTENER_H
