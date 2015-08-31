/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_AUDIOLISTENER_H
#define RE_AUDIOLISTENER_H

#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Base/ECS/Component.h"

namespace re {

class AudioListenerImpl;
class AudioListener : public Component<AudioListener>
{
public:
    AudioListener();
    virtual ~AudioListener();

public:
    float getGain() const;
    const Vec3& getPosition() const;
    const Quat& getRotation() const;
    const Vec3& getVelocity() const;

    void setGain(float gain);
    void setPosition(const Vec3& pos);
    void setRotation(const Quat& rot);
    void setVelocity(const Vec3& vel);

protected:
    float gain;

    Vec3 position;
    Quat rotation;
    Vec3 velocity;

    SharedPtr<AudioListenerImpl> impl;
};

class AudioListenerImpl
{
public:
    AudioListenerImpl() = default;
    virtual ~AudioListenerImpl() {};

    virtual void setGain(float gain) = 0;
    virtual void setPosition(const Vec3& pos) = 0;
    virtual void setRotation(const Quat& rot) = 0;
    virtual void setVelocity(const Vec3& vel) = 0;

protected:
    ComponentHandle<AudioListener> component;
};

} // namespace re

#endif // RE_AUDIOLISTENER_H
