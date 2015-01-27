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

    this->tweenManager = nullptr;
}

GameHub::~GameHub()
{
    if (tweenManager) {
        delete tweenManager;
    }
}

void GameHub::init(ApplicationProtocol *app)
{
    this->app = app;

    TextureManager::getInstance().setImageLoader(new ImageLoader());

    this->tweenManager = new TweenEngine::TweenManager();
    PredefineTweenAccessor::registerAccessor();
}

void GameHub::mainLoop(int64_t dt)
{
    this->updateFps(dt, fps);

    this->_lastSpanTime.SetTime(dt);
    this->_gameTime.SetTime(_gameTime.GetMilliSecond() + dt);

    if (tweenManager) {
        tweenManager->update(dt/1000.0);
    }

    this->updateEvent.emit();

    SceneManager::getInstance().renderScene();

}

Renderer &GameHub::GetRenderer()
{
    return SceneManager::getInstance().getRenderManager().getRenderer();
}

TweenEngine::TweenManager& GameHub::GetTweenManager()
{
    return *(this->tweenManager);
}

ApplicationProtocol &GameHub::GetApp()
{
    return *(this->app);
}

const Time &GameHub::GetGameTime() const
{
    return _gameTime;
}

const Time &GameHub::GetDeltaTime() const
{
    return _lastSpanTime;
}

int GameHub::GetFps()
{
    return this->fps;
}

int64_t time_ = 0;
int frame_ = 0;
void GameHub::updateFps(int64_t dt, int &fps)
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
