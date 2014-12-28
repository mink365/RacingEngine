#include "GameHub.h"

#include "Scene/SceneManager.h"
#include "Texture/TextureManager.h"
#include "Render/Renderer/Renderer.h"
#include "SceneManager.h"
#include "Render/RenderManager.h"
#include "TweenManager.h"
#include "Util/PredefineTweenAccessor.h"

namespace re {

GameHub::GameHub()
{
    fps = 0;

    this->updateFunc = nullptr;
    this->tweenManager = nullptr;
}

GameHub::~GameHub()
{
    if (tweenManager) {
        delete tweenManager;
    }
}

void GameHub::init()
{
    TextureManager::getInstance().setImageLoader(new ImageLoader());

    this->tweenManager = new TweenEngine::TweenManager();
    PredefineTweenAccessor::registerAccessor();
}

void GameHub::mainLoop(long dt)
{
    this->updateFps(dt, fps);

    this->_lastSpanTime.SetTime(dt);
    this->_gameTime.SetTime(_gameTime.GetMilliSecond() + dt);

    if (tweenManager) {
        tweenManager->update(dt/1000.0);
    }

    SceneManager::getInstance().renderScene();

    if (this->updateFunc) {
        this->updateFunc(dt);
    }
}

void GameHub::bindUpdateFunc(std::function<void (long)> func)
{
    this->updateFunc = func;
}

Renderer &GameHub::GetRenderer()
{
    return SceneManager::getInstance().getRenderManager().getRenderer();
}

TweenEngine::TweenManager& GameHub::getTweenManager()
{
    return *(this->tweenManager);
}

const Time &GameHub::GetGameTime() const
{
    return _gameTime;
}

const Time &GameHub::GetDeltaTime() const
{
    return _lastSpanTime;
}

int GameHub::getFps()
{
    return this->fps;
}

long time_ = 0;
int frame_ = 0;
void GameHub::updateFps(long dt, int &fps)
{
    time_ += dt;
    frame_ += 1;
    if (time_ > 1000) {
        fps = frame_;

        time_ = 0;
        frame_ = 0;
    }
}

} // namespace re
