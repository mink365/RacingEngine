#include "FeatureTestsApp.h"

#include "UI/Layout/Screen.h"
#include "Platform/GameHub.h"
#include "Message/MessageManager.h"
#include "Scene/SceneManager.h"
#include "Camera/Camera.h"
#include "FileSystem/FileSystem.h"
#include "Texture/TextureManager.h"
#include "Texture/TextureParser.h"
#include "Shader/ShaderUtil.h"
#include "Shader/ShaderManager.h"
#include "Renderer/GLES2Renderer.h"
#include "UI/Manager/UISceneManager.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "opengl.h"

#include "FontTest/FontTest.h"
#include "FBXTest/FBXTest.h"
#include "TweenTest/TweenTest.h"
#include "UITest/UITest.h"
#include "MotoSceneTest/SceneMaterialTest.h"
#include "LightTest/LightTest.h"
#include "ShadowTest/ShadowTest.h"

FeatureTestsApp::FeatureTestsApp()
{
}

bool FeatureTestsApp::initEnvironment()
{
    Screen::getInstance().setDesignSize(614, 1024);

    Screen::getInstance().setRealFrameSize(this->view->getFrameSize());

    std::function<void(long)> func = std::bind(&FeatureTestsApp::update, this, std::placeholders::_1);
    GameHub::getInstance().bindUpdateFunc(func);

    this->initResources();
    this->createTests();

    return true;
}

void FeatureTestsApp::onEnterForeground()
{

}

void FeatureTestsApp::onExitForeground()
{

}

void FeatureTestsApp::createTests()
{
    // TODO:
    std::shared_ptr<BaseTest> test;
    test = std::dynamic_pointer_cast<BaseTest>(std::make_shared<FBXTest>());
    this->tests.push_back(test);
    test = std::dynamic_pointer_cast<BaseTest>(std::make_shared<FontTest>());
    this->tests.push_back(test);
    test = std::dynamic_pointer_cast<BaseTest>(std::make_shared<TweenTest>());
    this->tests.push_back(test);
    test = std::dynamic_pointer_cast<BaseTest>(std::make_shared<UITest>());
    this->tests.push_back(test);
    test = std::dynamic_pointer_cast<BaseTest>(std::make_shared<SceneMaterialTest>());
    this->tests.push_back(test);
    test = std::dynamic_pointer_cast<BaseTest>(std::make_shared<LightTest>());
    this->tests.push_back(test);
    test = std::dynamic_pointer_cast<BaseTest>(std::make_shared<ShadowTest>());
    this->tests.push_back(test);

    currIndex = 6;
    this->onCurrentTestChanged();
}

void FeatureTestsApp::nextTest()
{
    if (currIndex < tests.size() - 1) {
        currIndex ++;

        this->onCurrentTestChanged();
    }
}

void FeatureTestsApp::lastTest()
{
    if (currIndex > 0) {
        currIndex --;

        this->onCurrentTestChanged();
    }
}

void FeatureTestsApp::onCurrentTestChanged()
{
    this->current = tests[currIndex];

    this->rootNode->removeAllChildren();

    this->current->init(*this);
}

void FeatureTestsApp::initResources()
{
//    InitGLStates();
    auto& sceneManager = SceneManager::getInstance();
    RendererPtr renderer = std::make_shared<GLES2Renderer>();

    sceneManager.getRenderManager().setRenderer(renderer);
    sceneManager.getRenderManager().initDefaultRenderState();

    const Screen& screen = Screen::getInstance();

    presCamera = std::make_shared<Camera>();
    presCamera->setViewport(screen.getRealSize().width, screen.getRealSize().height);
    presCamera->setDepthField(10, 1320);
    presCamera->setView(Vec3(0, -340, 57), Vec3(0, 30, 20), Vec3(0, 0, 1));
    presCamera->setQueueCullFunc([](int queue) {
        if (queue == RENDER_QUEUE_UI) {
            return false;
        }
        return true;
    });
    CameraPtr uiCamera = std::make_shared<Camera>();
    uiCamera->setProjectionMode(CameraProjectionMode::Orthographic);
    uiCamera->setViewport(screen.getRealSize().width, screen.getRealSize().height);
    uiCamera->setOrthoWidth(screen.getWidth());
    uiCamera->setDepthField(-10, 10);
    uiCamera->setView(Vec3(screen.getWidth()/2.0, screen.getHeight()/2.0, 0),
                        Vec3(screen.getWidth()/2.0, screen.getHeight()/2.0, 1), Vec3(0, 1, 0));
    // TODO: no need it, but depth not clear
    uiCamera->setClearFlag(0);
    uiCamera->setQueueCullFunc([](int queue) {
        if (queue == RENDER_QUEUE_UI) {
            return true;
        }
        return false;
    });
    SceneManager::getInstance().getRenderManager().addCamera(presCamera);
    SceneManager::getInstance().getRenderManager().addCamera(uiCamera);

    SceneManager::getInstance().addRootNode(presCamera);
    SceneManager::getInstance().addRootNode(uiCamera);

    SearchPath searchPath;
    searchPath.rootDir = "/home/jk/workspace/engines/RacingEngine/Examples/Resources/";
    searchPath.type = FileType::Permanent;
    FileSystem::getInstance().addSearchPath(searchPath);

    std::string shaderDir = "Shaders/";

//    LoadShader("Shader_Default", shaderDir + "light.vert", shaderDir + "light.frag");

    LoadShader("Shader_PTC", shaderDir + "position_texture_color.vert",
                             shaderDir + "position_texture_color.frag");

    LoadShader("Shader_PTC_TM", shaderDir + "position_texture_color_texture_matrix.vert",
                             shaderDir + "position_texture_color_texture_matrix.frag");

    LoadShader("Shader_Font", shaderDir + "v3f-t2f-c4f.vert",
                              shaderDir + "v3f-t2f-c4f.frag");

    TextureManager::getInstance().setImageLoader(new ImageLoader());

    rootNode = std::make_shared<SceneNode>();
    SceneManager::getInstance().addRootNode(rootNode);

    this->stage = CreateView<ui::UISceneManager>();
    SceneManager::getInstance().addRootNode(this->stage);
    MessageManager::getInstance()->addHandler(this->stage.get());

    std::shared_ptr<ISceneFactory> factory = std::make_shared<SceneFactory>();
    this->stage->setSceneFactory(factory);
}

void FeatureTestsApp::update(long dt)
{
    MessageManager::getInstance()->handleMessages();

    if (this->current) {
        this->current->Update(dt / 1000.0f);
    }
}

std::shared_ptr<Window> WindowFactory::createView(const string& name)
{
    std::shared_ptr<Window> win = nullptr;
    if (name == "HelloWindow") {
        win = CreateView<Window>();
    }

    if (win) {
        win->setName(name);
    }

    return win;
}

std::shared_ptr<Scene> SceneFactory::createView(const string& name)
{
    std::shared_ptr<Scene> scene = nullptr;
    if (name == "Scene1") {
        scene = CreateView<Scene>();
    }

    if (scene) {
        std::shared_ptr<IWindowFactory> factory = std::make_shared<WindowFactory>();

        scene->setName(name);
        scene->setWindowFactory(factory);
    }

    return scene;
}
