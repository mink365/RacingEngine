/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_AUDIOSOURCE_H
#define RE_AUDIOSOURCE_H

#include "Math/Vector.h"
#include "Base/ECS/Component.h"
#include "PreDeclare.h"

namespace re {

enum class AudioSourceState
{
    INITIAL,
    PLAYING,
    PAUSED,
    STOPPED
};

class AudioSourceImpl;

class AudioSource : public Component<AudioSource>
{
public:
    AudioSource();
    virtual ~AudioSource();

public:
    void play();
    void pause();
    void resume();
    void stop();
    void rewind();

    bool isLooped() const;
    void setLooped(bool looped);

    AudioSourceState getState() const;

    float getGain() const;
    float getPitch() const;

    const Vec3& getPosition() const;
    const Vec3& getVelocity() const;

    void setGain(float gain);
    void setPitch(float pitch);
    void setPosition(const Vec3& pos);
    void setVelocity(const Vec3& vel);

    void bindFile(const std::string &path);

protected:
    void registerEvents();
    void onAwake();

public:
    SharedPtr<AudioSourceImpl> getImpl()
    {
        return impl;
    }

protected:
    bool looped;

    float gain;
    float pitch;

    Vec3 position;
    Vec3 velocity;

    SharedPtr<AudioSourceImpl> impl;
};

class AudioSourceImpl
{
    friend class AudioSource;
public:
    virtual ~AudioSourceImpl() {}

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;
    virtual void rewind() = 0;

    virtual void setLooped(bool looped) = 0;

    virtual AudioSourceState getState() const = 0;

    virtual void setGain(float gain) = 0;
    virtual void setPitch(float pitch) = 0;
    virtual void setPosition(const Vec3& pos) = 0;
    virtual void setVelocity(const Vec3& vel) = 0;

    virtual void bindFile(const std::string& path) = 0;

protected:
    ComponentHandle<AudioSource> component;
};

} // namespace re

#endif // RE_AUDIOSOURCE_H
