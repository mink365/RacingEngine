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
    OpenALAudioEngine::instance();

    auto buffer = OpenALAudioFactory::instance().CreateBuffer("Sound/Bomb.ogg");

    sourceNode = CreateNodeEntity();
    auto source = sourceNode->addComponent<AudioSource>();
    std::dynamic_pointer_cast<OpenALSource>(source->getImpl())->bindBuffer(buffer);

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

