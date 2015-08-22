/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "AudioTest.h"

#include "Util/ComponentFactory.h"

#include "Audio/OpenAL/OpenALSource.h"
#include "Audio/OpenAL/OpenALListener.h"
#include "Audio/OpenAL/OpenALBuffer.h"
#include "Audio/OpenAL/OpenALAudioFactory.h"
#include "Audio/OpenAL/OpenALAudioEngine.h"
#include "GameHub.h"

AudioTest::AudioTest()
{
    this->name = "AudioTest";
}

void AudioTest::Init()
{
    OpenALAudioEngine::getInstance();

    auto buffer = OpenALAudioFactory::getInstance().CreateBuffer("Sound/Bomb.ogg");

    sourceNode = CreateNode();
    auto source = sourceNode->addComponent<OpenALSource>();
    source->bindBuffer(buffer);

    listenerNode = CreateNode();
    listenerNode->addComponent<OpenALListener>();
}

float t = 0;
Vec3 pos;

void AudioTest::Update()
{
    float dt = GameHub::getInstance().GetDeltaTime().GetSecond();
    t += dt;

    if (t > 3) {
        sourceNode->getComponent<OpenALSource>()->play();

        t = 0;
    }

    pos += Vec3(-1, 0, 0) * dt * 3;

    listenerNode->getComponent<OpenALListener>()->setPosition(pos);
}

