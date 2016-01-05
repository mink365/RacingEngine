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
#include "UI/Base/Text.h"
#include "UI/Widget/Button.h"
#include "UI/Layout/LayoutUtil.h"
#include "UI/Rendering/DebugRenderer.h"
#include "UI/Rendering/ElementBatcher.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "opengl.h"

#include "Font/FontTest.h"
#include "FBX/FBXTest.h"
#include "Tween/TweenTest.h"
#include "UI/UITest.h"
#include "UI/LayoutTest.h"
#include "UI/MaskTest.h"
#include "UI/ScrollViewTest.h"
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
#include "Serialization/SerializationTest.h"
#include "Resource/ResourceManager.h"

#include "Util/Logging.h"
#include "Util/PredefineTweenAccessor.h"
#include "Util/EventUtil.h"

extern TextureAtlasPtr CreateDefaultFont();

FeatureTestsApp::FeatureTestsApp()
{
}

bool FeatureTestsApp::initEnvironment()
{
    PredefineTweenAccessor::registerAccessor();
    EventFactory::RegisterEvents();

    Screen::instance().setDesignSize(614, 1024);

    Screen::instance().setRealFrameSize(this->view->getFrameSize());

    GameHub::instance().updateEvent += std::bind(&FeatureTestsApp::update, this);

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

template<class T>
void CreateTest(std::vector<BaseTestPtr>& tests)
{
    auto test = std::make_shared<T>();
    tests.push_back(test);
}

template<class T, class T2, class... TL>
void CreateTest(std::vector<BaseTestPtr>& tests)
{
    auto test = std::make_shared<T>();
    tests.push_back(test);

    CreateTest<T2, TL...>(tests);
}

void FeatureTestsApp::createTests()
{
    CreateTest<FBXTest, AudioTest, FontTest, TweenTest
            , UITest, LayoutTest, MaskTest, ScrollViewTest
            , SerializationTest
            , SceneMaterialTest, LightTest
            , ShadowTest, BulletTest, RenderToTextureTest, Skybox
            , Reflection, Refraction, Lightmap, Bumpmap>(this->tests);

    currIndex = 7;

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
    SceneManager::instance().getRenderManager().initDefaultRenderState();
    SceneManager::instance().getRenderManager().lightList.clear();

    this->current = tests[currIndex];

    this->stage->getLastLayer()->getComponent<WindowManager>()->popAllWindow();
    this->rootNode->removeAllChildren();

    this->current->init(*this);
    this->labelTitle->setText(this->current->getName());

    SceneManager::instance().getRenderManager().markRenderViewDirty();
    SceneManager::instance().getRenderManager().initDefaultRenderState();
}

void FeatureTestsApp::createBaseUI()
{
    TextureParser::instance().addTextures("UI/", "png");

    TextureManager::instance().loadTextures();

    auto font = FontManager::instance().GetResource("default");
    if (font == nullptr) {
        CreateDefaultFont();

        font = FontManager::instance().GetResource("default");
    }

    labelTitle = CreateNode2D<Text>();
    labelTitle->getComponent<Transform2D>()->setAnchor(Vec2(0.5,0.5));
    labelTitle->setText("Hello");

    labelFps = CreateNode2D<Text>();
    labelFps->setText("HelleH");

    auto buttonNext = CreateImageButton("b_you.png", "b_you.png", "b_you.png");
    auto buttonPrev = CreateImageButton("b_zuo.png", "b_zuo.png", "b_zuo.png");

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
    auto& sceneManager = SceneManager::instance();
    RendererPtr renderer = Create<GLES2Renderer>();

    sceneManager.getRenderManager().setRenderer(renderer);
    sceneManager.getRenderManager().initDefaultRenderState();

    const Screen& screen = Screen::instance();

    presCamera = CreateNode<Camera>();
    presCamera->setViewport(screen.getRealSize().width, screen.getRealSize().height);
    presCamera->setDepthField(10, 1320);
    presCamera->setView(Vec3(0, -340, 57), Vec3(0, 30, 20), Vec3(0, 0, 1));
    presCamera->setQueueCullFunc([](int queue) {
        if (queue == RenderQueues::UI) {
            return false;
        }
        return true;
    });

    CameraPtr uiCamera = CreateNode<Camera>();
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
    SceneManager::instance().getRenderManager().addCamera(presCamera);
    SceneManager::instance().getRenderManager().addCamera(uiCamera);

    SceneManager::instance().addRootNode(presCamera->getNode());
    SceneManager::instance().addRootNode(uiCamera->getNode());

    SearchPath searchPath;
#ifdef RE_PLATFORM_LINUX
    searchPath.dir = "/home/jk/workspace/RacingEngine/Examples/Resources/";
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

    TextureManager::instance().setImageLoader(new ImageLoader());

    DebugRenderer::instance().InitNode();

    rootNode = CreateNodeEntity()->getNode();
    SceneManager::instance().addRootNode(rootNode);

    this->stage = CreateNode2D<ui::UIManager>();
    this->stage->getEntity()->addComponent<ElementBatcher>();
    SceneManager::instance().addRootNode(this->stage->getNode());
    MessageManager::instance().addHandler(this->stage.get());

    this->registerScenes();
    this->registerWindows();

    this->createBaseUI();
}

void FeatureTestsApp::registerWindows()
{
    stage->getWindowFactory().registerCreateFunc("HelloWindow", [](){
        auto window = CreateNode2D<Window>();
        window->setName("HelloWindow");
        return window;
    });
}

void FeatureTestsApp::registerScenes()
{
    stage->getSceneFactory().registerCreateFunc("Scene1", [](){
        auto scene = CreateNode2D<Scene>();
        scene->setName("Scene1");
        return scene;
    });
}

void FeatureTestsApp::update()
{
    MessageManager::instance().handleMessages();

    if (this->current) {
        this->current->Update();
    }
    this->stage->update();

    this->labelFps->setText(std::to_string(GameHub::instance().GetFps()));

    auto node = stage->getNode();
    DebugRenderer::instance().Step(node);
}

#ifdef __cplusplus
extern "C" {
#endif
    
    void application_init()
    {
        Application* app = new FeatureTestsApp();
        
        app->initViewSize();
        
        GameHub& game = GameHub::instance();
        game.init(app);
        
        app->initEnvironment();
    }
    
    void application_update()
    {
        GameHub& game = GameHub::instance();
        
        int64_t dt = 10;
        game.mainLoop(dt);
    }
    
#ifdef __cplusplus
}
#endif
