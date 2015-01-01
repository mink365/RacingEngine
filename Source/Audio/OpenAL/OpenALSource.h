/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_OPENALSOURCE_H
#define RE_OPENALSOURCE_H

#include "Audio/AudioSource.h"
#include "OpenALBuffer.h"
#include "OpenALBase.h"

namespace re {

class OpenALSource : public AudioSource
{
public:
    OpenALSource(OpenALBuffer::ptr buffer, ALuint source);
    ~OpenALSource();

public:
    virtual void play() override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void stop() override;
    virtual void rewind() override;

    virtual void setLooped(bool looped) override;

    virtual AudioSourceState getState() const;

    virtual void setGain(float gain) override;
    virtual void setPitch(float pitch) override;
    virtual void setPosition(const Vec3& pos) override;
    virtual void setVelocity(const Vec3& vel) override;

private:
    ALuint _alSource;
    OpenALBuffer::ptr _buffer;
};

} // namespace re

#endif // RE_OPENALSOURCE_H