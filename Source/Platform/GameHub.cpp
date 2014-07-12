#include "GameHub.h"

#include "Scene/SceneManager.h"
#include "Texture/TextureManager.h"

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
    this->updateFunc(dt);

    SceneManager::getInstance().renderScene();
}

void GameHub::bindUpdateFunc(std::function<void (long)> func)
{
    this->updateFunc = func;
}

} // namespace re
