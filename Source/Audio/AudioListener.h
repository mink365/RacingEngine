/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_AUDIOLISTENER_H
#define RE_AUDIOLISTENER_H

#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Scene/Component.h"

namespace re {

class AudioListener : public Component
{
public:
    AudioListener();
    virtual ~AudioListener();

public:
    float getGain() const;
    const Vec3& getPosition() const;
    const Quat& getRotation() const;
    const Vec3& getVelocity() const;

    virtual void setGain(float gain);
    virtual void setPosition(const Vec3& pos);
    virtual void setRotation(const Quat& rot);
    virtual void setVelocity(const Vec3& vel);

protected:
    float gain;

    Vec3 position;
    Quat rotation;
    Vec3 velocity;
};

} // namespace re

#endif // RE_AUDIOLISTENER_H
