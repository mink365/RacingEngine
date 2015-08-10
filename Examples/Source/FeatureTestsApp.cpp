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
#include "Renderer/GLES2Renderer.h"
#include "UI/Manager/UIManager.h"
#include "UI/Base/Label.h"
#include "UI/Widget/Button.h"
#include "UI/Layout/LayoutUtil.h"
#include "UI/Rendering/DebugRenderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "opengl.h"

#include "Font/FontTest.h"
#include "FBX/FBXTest.h"
#include "Tween/TweenTest.h"
#include "UI/UITest.h"
#include "UI/LayoutTest.h"
#include "MotoScene/SceneMaterialTest.h"
#include "Light/LightTest.h"
#include "Shadow/ShadowTest.h"
#include "Bullet/BulletTest.h"
#include "RenderToTexture/RenderToTextureTest.h"
#include "Skybox/Skybox.h"
#include "Reflection/Reflection.h"
#include "Refraction/Refraction.h"
#include "Lightmap/Lightmap.h"
#include "Bumpmap/Bumpmap.h"
#include "Audio/AudioTest.h"
#include "Resource/ResourceManager.h"

#include "Util/LogUtil.h"

extern std::shared_ptr<TextureAtlas> CreateDefaultFont();

FeatureTestsApp::FeatureTestsApp()
{
}

bool FeatureTestsApp::initEnvironment()
{
    Screen::getInstance().setDesignSize(614, 1024);

    Screen::getInstance().setRealFrameSize(this->view->getFrameSize());

    GameHub::getInstance().updateEvent += std::bind(&FeatureTestsApp::update, this);

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
    test = std::make_shared<AudioTest>();
    this->tests.push_back(test);
    test = std::make_shared<FontTest>();
    this->tests.push_back(test);
    test = std::make_shared<TweenTest>();
    this->tests.push_back(test);
    test = std::make_shared<UITest>();
    this->tests.push_back(test);
    test = std::make_shared<LayoutTest>();
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
    test = std::make_shared<Bumpmap>();
    this->tests.push_back(test);

    currIndex = 5;

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

    this->stage->getLastLayer()->getComponent<WindowManager>()->popAllWindow();
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

    std::shared_ptr<Font> font = FontManager::getInstance().GetResource("default");
    if (font == nullptr) {
        CreateDefaultFont();

        font = FontManager::getInstance().GetResource("default");
    }

    labelTitle = CreateNode2DComponent<Label>(font);
    labelTitle->getComponent<Transform2D>()->setAnchorPoint(Vec2(0.5,0.5));
    labelTitle->setText("Hello");

    labelFps = CreateNode2DComponent<Label>(font);
    labelFps->setText("HelleH");

    auto buttonNext = CreateNode2DComponent<ImageButton>("b_you.png", "b_you.png", "b_you.png");
    auto buttonPrev = CreateNode2DComponent<ImageButton>("b_zuo.png", "b_zuo.png", "b_zuo.png");

    auto buttonClickFunc = [=](ButtonPtr& widget) {
        if (widget == buttonNext) {
            this->nextTest();
        } else if (widget == buttonPrev) {
            this->lastTest();
        }
    };

    buttonNext->setOnClickFunc(buttonClickFunc);
    buttonPrev->setOnClickFunc(buttonClickFunc);

    auto scene = stage->pushTo("Scene1");
    auto sceneNode = scene->getNode();
    sceneNode->addChild(labelTitle->getNode());
    sceneNode->addChild(labelFps->getNode());
    sceneNode->addChild(buttonNext->getNode());
    sceneNode->addChild(buttonPrev->getNode());

    LayoutUtil::LayoutToParent(labelTitle->getComponent<Transform2D>(), AlignType::CENTER_TOP, AlignType::CENTER_TOP, 0, -80);
    LayoutUtil::LayoutToParent(labelFps->getComponent<Transform2D>(), AlignType::LEFT_BOTTOM, AlignType::LEFT_BOTTOM);
    LayoutUtil::LayoutToParent(buttonPrev->getComponent<Transform2D>(), AlignType::LEFT_CENTER, AlignType::LEFT_CENTER);
    LayoutUtil::LayoutToParent(buttonNext->getComponent<Transform2D>(), AlignType::RIGHT_CENTER, AlignType::RIGHT_CENTER);
}

void FeatureTestsApp::initResources()
{
//    InitGLStates();
    auto& sceneManager = SceneManager::getInstance();
    RendererPtr renderer = std::make_shared<GLES2Renderer>();

    sceneManager.getRenderManager().setRenderer(renderer);
    sceneManager.getRenderManager().initDefaultRenderState();

    const Screen& screen = Screen::getInstance();

    auto presCameraNode = CreateComponent<Camera>();
    presCamera = presCameraNode->getComponent<Camera>();
    presCamera->setViewport(screen.getRealSize().width, screen.getRealSize().height);
    presCamera->setDepthField(10, 1320);
    presCamera->setView(Vec3(0, -340, 57), Vec3(0, 30, 20), Vec3(0, 0, 1));
    presCamera->setQueueCullFunc([](int queue) {
        if (queue == RenderQueues::UI) {
            return false;
        }
        return true;
    });

    auto uiCameraNode = CreateComponent<Camera>();
    CameraPtr uiCamera = uiCameraNode->getComponent<Camera>();
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
        if (queue == RenderQueues::UI) {
            return true;
        }
        return false;
    });
    SceneManager::getInstance().getRenderManager().addCamera(presCamera);
    SceneManager::getInstance().getRenderManager().addCamera(uiCamera);

    SceneManager::getInstance().addRootNode(presCamera->getNode());
    SceneManager::getInstance().addRootNode(uiCamera->getNode());

    SearchPath searchPath;
#ifdef RE_PLATFORM_LINUX
    searchPath.dir = "/home/jk/workspace/engines/RacingEngine/Examples/Resources/";
#elif RE_PLATFORM_ANDROID
//    searchPath.dir = "/sdcard/regame/assets/";
    searchPath.dir = "asset:";
#elif RE_PLATFORM_MAC
    searchPath.dir = "/Users/ruikou/workspace/RacingEngine/Examples/Resources/";
#endif
    FileSystem::getInstance().addSearchPath(searchPath);

    std::string shaderDir = "Shaders/";

//    LoadShader("Shader_Default", shaderDir + "light.vert", shaderDir + "light.frag");

    LoadShader("Shader_Debug", shaderDir + "debug.vert",
                             shaderDir + "debug.frag");

    LoadShader("Shader_PTC", shaderDir + "position_texture_color.vert",
                             shaderDir + "position_texture_color.frag");

    LoadShader("Shader_PTC_TM", shaderDir + "position_texture_color_texture_matrix.vert",
                             shaderDir + "position_texture_color_texture_matrix.frag");

    LoadShader("Shader_Font", shaderDir + "v3f-t2f-c4f.vert",
                              shaderDir + "v3f-t2f-c4f.frag");

    TextureManager::getInstance().setImageLoader(new ImageLoader());

    DebugRenderer::getInstance().InitNode();

    rootNode = CreateNode();
    SceneManager::getInstance().addRootNode(rootNode);

    this->stage = CreateNode2DComponent<ui::UIManager>();
    SceneManager::getInstance().addRootNode(this->stage->getNode());
    MessageManager::getInstance().addHandler(this->stage.get());

    this->registerScenes();
    this->registerWindows();

    this->createBaseUI();
}

void FeatureTestsApp::registerWindows()
{
    stage->getWindowFactory().registerCreateFunc("HelloWindow", [](){
        auto window = CreateNode2DComponent<Window>();
        window->setName("HelloWindow");
        return window;
    });
}

void FeatureTestsApp::registerScenes()
{
    stage->getSceneFactory().registerCreateFunc("Scene1", [](){
        auto scene = CreateNode2DComponent<Scene>();
        scene->setName("Scene1");
        return scene;
    });
}

void FeatureTestsApp::update()
{
    MessageManager::getInstance().handleMessages();

    if (this->current) {
        this->current->Update();
    }
    this->stage->update();

    this->labelFps->setText(std::to_string(GameHub::getInstance().GetFps()));

    auto node = stage->getNode();
    DebugRenderer::getInstance().Step(node);
}
