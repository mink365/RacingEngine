#include "GameHub.h"

#include "Scene/SceneManager.h"
#include "Texture/TextureManager.h"
#include "Render/Renderer/Renderer.h"
#include "SceneManager.h"
#include "Render/RenderManager.h"

namespace re {

GameHub::GameHub()
{
    this->updateFunc = [](long dt) {};
}

void GameHub::init()
{
    TextureManager::getInstance().setImageLoader(new ImageLoader());
}

void GameHub::mainLoop(long dt)
{
//    this->updateFunc(dt);

    SceneManager::getInstance().renderScene();

    this->updateFunc(dt);
}

void GameHub::bindUpdateFunc(std::function<void (long)> func)
{
    this->updateFunc = func;
}

Renderer &GameHub::GetRenderer()
{
    return SceneManager::getInstance().getRenderManager().getRenderer();
}

} // namespace re
