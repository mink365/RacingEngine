/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef AUDIOTEST_H
#define AUDIOTEST_H

#include "BaseTest.h"
#include "Audio/AudioSource.h"
#include "Audio/AudioListener.h"

class AudioTest : public BaseTest
{
public:
    AudioTest();

    virtual void Init();
    virtual void Update(float dt) override;

private:
    std::shared_ptr<AudioSource> source;
    std::shared_ptr<AudioListener> listener;
};

#endif // AUDIOTEST_H
