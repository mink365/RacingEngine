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
    _time = 0;
    _frame = 0;
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
    _time += dt;
    _frame += 1;
    if (_time > 1000) {
        this->fps = _frame;

        _time = 0;
        _frame = 0;
    }

//    this->updateFunc(dt);
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

int GameHub::getFps()
{
    return this->fps;
}

} // namespace re
