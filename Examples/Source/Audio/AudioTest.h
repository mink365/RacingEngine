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
    virtual void Update() override;

private:
    EntityPtr sourceNode;
    EntityPtr listenerNode;
};

#endif // AUDIOTEST_H
