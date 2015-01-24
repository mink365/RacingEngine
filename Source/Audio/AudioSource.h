/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_AUDIOSOURCE_H
#define RE_AUDIOSOURCE_H

#include "Math/Vector.h"
#include "Scene/Component.h"

namespace re {

enum class AudioSourceState
{
    INITIAL,
    PLAYING,
    PAUSED,
    STOPPED
};

class AudioSource : public Component
{
public:
    AudioSource();
    virtual ~AudioSource();

public:
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;
    virtual void rewind() = 0;

    bool isLooped() const;
    virtual void setLooped(bool looped);

    virtual AudioSourceState getState() const = 0;

    float getGain() const;
    float getPitch() const;

    const Vec3& getPosition() const;
    const Vec3& getVelocity() const;

    virtual void setGain(float gain);
    virtual void setPitch(float pitch);
    virtual void setPosition(const Vec3& pos);
    virtual void setVelocity(const Vec3& vel);

protected:
    float looped;

    float gain;
    float pitch;

    Vec3 position;
    Vec3 velocity;
};

} // namespace re

#endif // RE_AUDIOSOURCE_H
