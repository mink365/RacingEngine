/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "AudioTest.h"

#include "Util/ComponentFactory.h"

#include "Audio/AudioEngine.h"
#include "Audio/AudioSource.h"
#include "Audio/AudioListener.h"
#include "GameHub.h"

AudioTest::AudioTest()
{
    this->name = "AudioTest";
}

void AudioTest::Init()
{
    AudioEngine::instance();

    sourceNode = CreateNodeEntity();
    auto source = sourceNode->addComponent<AudioSource>();
    source->bindFile("Sound/Bomb.ogg");

    listenerNode = CreateNodeEntity();
    listenerNode->addComponent<AudioListener>();
}

float t = 0;
Vec3 pos;

void AudioTest::Update()
{
    float dt = GameHub::instance().GetDeltaTime().GetSecond();
    t += dt;

    if (t > 3) {
        sourceNode->getComponent<AudioSource>()->play();

        t = 0;
    }

    pos += Vec3(-1, 0, 0) * dt * 3;

    listenerNode->getComponent<AudioListener>()->setPosition(pos);
}

