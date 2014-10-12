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
#include "UI/Manager/UIManager.h"
#include "UI/Base/Label.h"
#include "UI/Base/Node2d.h"
#include "UI/Widget/Button.h"
#include "UI/Layout/LayoutUtil.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "opengl.h"

#include "Font/FontTest.h"
#include "FBX/FBXTest.h"
#include "Tween/TweenTest.h"
#include "UI/UITest.h"
#include "MotoScene/SceneMaterialTest.h"
#include "Light/LightTest.h"
#include "Shadow/ShadowTest.h"
#include "Bullet/BulletTest.h"
#include "RenderToTexture/RenderToTextureTest.h"
#include "Skybox/Skybox.h"
#include "Reflection/Reflection.h"
#include "Refraction/Refraction.h"
#include "Lightmap/Lightmap.h"

#include "Util/LogUtil.h"

extern std::shared_ptr<TextureAtlas> CreateDefaultFont();

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
    std::shared_ptr<BaseTest> test;
    test = std::make_shared<FBXTest>();
    this->tests.push_back(test);
    test = std::make_shared<FontTest>();
    this->tests.push_back(test);
    test = std::make_shared<TweenTest>();
    this->tests.push_back(test);
    test = std::make_shared<UITest>();
    this->tests.push_back(test);
    test = std::make_shared<SceneMaterialTest>();
    this->tests.push_back(test);
    test = std::make_shared<LightTest>();
    this->tests.push_back(test);
    test = std::make_shared<ShadowTest>();
    this->tests.push_back(test);
    test = std::make_shared<BulletTest>();
    this->tests.push_back(test);
    test = std::make_shared<RenderToTextureTest>();
    this->tests.push_back(test);
    test = std::make_shared<Skybox>();
    this->tests.push_back(test);
    test = std::make_shared<Reflection>();
    this->tests.push_back(test);
    test = std::make_shared<Refraction>();
    this->tests.push_back(test);
    test = std::make_shared<Lightmap>();
    this->tests.push_back(test);

    currIndex = 1;

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
    SceneManager::getInstance().getRenderManager().initDefaultRenderState();
    SceneManager::getInstance().getRenderManager().lightList.clear();

    this->current = tests[currIndex];

    this->stage->getLastLayer()->popAllWindow();
    this->rootNode->removeAllChildren();

    this->current->init(*this);
    this->labelTitle->setText(this->current->getName());

    SceneManager::getInstance().getRenderManager().markRenderViewDirty();
    SceneManager::getInstance().getRenderManager().initDefaultRenderState();
}

void FeatureTestsApp::createBaseUI()
{
    TextureParser::getInstance().addTextures("UI/", "png");
    TextureManager::getInstance().loadTextures();

    std::shared_ptr<Font> font = FontManager::getInstance().getFont("default");
    if (font == nullptr) {
        CreateDefaultFont();

        font = FontManager::getInstance().getFont("default");
    }

    labelTitle = std::make_shared<Label>();
    labelTitle->init(font);
    labelTitle->setAnchorPoint(Vec2(0.5,0.5));
    labelTitle->setText("Hello");

    labelFps = std::make_shared<Label>();
    labelFps->init(font);
    labelFps->setText("HelleH");

    auto buttonNext = CreateView<ImageButton>();
    buttonNext->initView("b_you.png", "b_you.png", "b_you.png");
    auto buttonPrev = CreateView<ImageButton>();
    buttonPrev->initView("b_zuo.png", "b_zuo.png", "b_zuo.png");

    auto buttonClickFunc = [=](WidgetPtr& widget) {
        if (widget == buttonNext) {
            this->nextTest();
        } else if (widget == buttonPrev) {
            this->lastTest();
        }
    };

    buttonNext->setOnClickFunc(buttonClickFunc);
    buttonPrev->setOnClickFunc(buttonClickFunc);

    auto scene = stage->pushTo("Scene1");
    scene->addChild(labelTitle);
    scene->addChild(labelFps);
    scene->addChild(buttonNext);
    scene->addChild(buttonPrev);

    LayoutUtil::layoutParent(labelTitle, AlignType::CENTER_TOP, AlignType::CENTER_TOP, 0, -80);
    LayoutUtil::layoutParent(labelFps, AlignType::LEFT_BOTTOM, AlignType::LEFT_BOTTOM);
    LayoutUtil::layoutParent(buttonPrev, AlignType::LEFT_CENTER, AlignType::LEFT_CENTER);
    LayoutUtil::layoutParent(buttonNext, AlignType::RIGHT_CENTER, AlignType::RIGHT_CENTER);
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
    uiCamera->setView(Vec3(screen.getWidth()/2.0, screen.getHeight()/2.0, 1),
                        Vec3(screen.getWidth()/2.0, screen.getHeight()/2.0, 0), Vec3(0, 1, 0));
    // TODO: why need this? or no depth clear for all 3d scene
    uiCamera->setBuffersClearFlag(ClearBufferBit::DEPTH_BUFFER_BIT
                                  | ClearBufferBit::STENCIL_BUFFER_BIT);
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
#ifdef RE_PLATFORM_LINUX
    searchPath.rootDir = "/home/jk/workspace/engines/RacingEngine/Examples/Resources/";
#elif RE_PLATFORM_ANDROID
    searchPath.rootDir = "/sdcard/regame/assets/";
#elif RE_PLATFORM_MAC
    searchPath.rootDir = "/Users/ruikou/Workspace/engine/RacingEngine/Examples/Resources/";
#endif
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

    this->stage = CreateView<ui::UIManager>();
    SceneManager::getInstance().addRootNode(this->stage);
    MessageManager::getInstance()->addHandler(this->stage.get());

    std::shared_ptr<ISceneFactory> factory = std::make_shared<SceneFactory>();
    this->stage->setSceneFactory(factory);

    this->createBaseUI();
}

void FeatureTestsApp::update(long dt)
{
    MessageManager::getInstance()->handleMessages();

    if (this->current) {
        this->current->Update(dt / 1000.0f);
    }
    this->stage->update(dt/1000.0f);

    this->labelFps->setText(std::to_string(GameHub::getInstance().getFps()));
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
