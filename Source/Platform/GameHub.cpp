#include "GameHub.h"

#include "Scene/SceneManager.h"
#include "Texture/TextureManager.h"
#include "Render/Renderer/Renderer.h"
#include "SceneManager.h"
#include "Render/RenderManager.h"

namespace re {

GameHub::GameHub()
{
    _time = 0;
    _frame = 0;
    fps = 0;

    this->updateFunc = nullptr;
}

void GameHub::init()
{
    TextureManager::getInstance().setImageLoader(new ImageLoader());
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

int GameHub::getFps()
{
    return this->fps;
}

} // namespace re
