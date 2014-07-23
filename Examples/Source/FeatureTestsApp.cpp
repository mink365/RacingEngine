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

    currIndex = 4;
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

int CheckShaderLinkError(GLint program) {
    GLint IsLinked;
    glGetProgramiv(program, GL_LINK_STATUS, (GLint *)&IsLinked);
    if(IsLinked==FALSE)
    {
        LOG_E("Failed to link shader.");

        GLint maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        if(maxLength > 0)
        {
            char *pLinkInfoLog = new char[maxLength];
            glGetProgramInfoLog(program, maxLength, &maxLength, pLinkInfoLog);
            LOG_E("shader log: %s\n", pLinkInfoLog);

            delete [] pLinkInfoLog;
        }

        return -1;
    }

    return 0;
}

void setLightShaderAttribute(Shader::ptr& shader) {
    // set vertex attribute
    Attribute *vertAttr = shader->getAttribute("aPosition");
    vertAttr->setType(ATTR_FORMAT_FLOAT);
    vertAttr->setSize(3);
    vertAttr->setStride(sizeof(Vertex));
    vertAttr->setOffset(0);

    Attribute *uvAttr = shader->getAttribute("aTexCoord");
    uvAttr->setType(ATTR_FORMAT_FLOAT);
    uvAttr->setSize(2);
    uvAttr->setStride(sizeof(Vertex));
    uvAttr->setOffset((3) * 4);

//    Attribute *colorAttr = shader.getAttribute("color");
//    colorAttr->setType(ATTR_FORMAT_FLOAT);
//    colorAttr->setSize(4);
//    colorAttr->setStride(sizeof(Vertex));
//    colorAttr->setOffset((8) * 4);

      // TODO:
//    Attribute *normalAttr = shader.getAttribute("aNormal");
//    normalAttr->setType(ATTR_FORMAT_FLOAT);
//    normalAttr->setSize(3);
//    normalAttr->setStride(sizeof(Vertex));
//    normalAttr->setOffset((5) * 4);
}

void setPTCShaderAttribute(Shader::ptr& shader) {
    // set vertex attribute
    Attribute *vertAttr = shader->getAttribute("aPosition");
    vertAttr->setType(ATTR_FORMAT_FLOAT);
    vertAttr->setSize(3);
    vertAttr->setStride(sizeof(Vertex));
    vertAttr->setOffset(0);

    Attribute *uvAttr = shader->getAttribute("aTexCoord");
    uvAttr->setType(ATTR_FORMAT_FLOAT);
    uvAttr->setSize(2);
    uvAttr->setStride(sizeof(Vertex));
    uvAttr->setOffset((3) * 4);

    Attribute *colorAttr = shader->getAttribute("aColor");
    colorAttr->setType(ATTR_FORMAT_FLOAT);
    colorAttr->setSize(4);
    colorAttr->setStride(sizeof(Vertex));
    colorAttr->setOffset((8) * 4);
}

Camera camera;
int LoadShaderData(const std::string& name, const std::string& vs, const std::string& fs) {
    Shader::ptr shader = Shader::create();
    shader->setName(name);

    shader->setVertexSource(vs);
    shader->setFragmentSource(fs);

    ShaderUtil::getInstance().compileShader(shader.get());

    CheckShaderLinkError(shader->getProgram());

    if (name == "Shader_Default") {
        setLightShaderAttribute(shader);
    } else {
        setPTCShaderAttribute(shader);
    }

//    shader.getUniform("model")->setData((float*)model);
//    shader->getUniform("view")->setData((float*)camera.getViewMatrix());
//    shader->getUniform("projection")->setData((float*)camera.getProjectionMatrix());

    ShaderManager::getInstance().registerShader(shader);

    return 1;
}

std::string loadFile(const char *fname)
{
    FilePtr file = FileSystem::getInstance().openFile(fname);
    Buffer::ptr buf = file->read();

    string data((char*)(buf->getData()), buf->getSize());
    return data;
}

int LoadShader(const std::string& name, const std::string& pfilePath_vs, const std::string& pfilePath_fs)
{
    // load shaders & get length of each
    std::string vertexShaderString = loadFile(pfilePath_vs.c_str());
    std::string fragmentShaderString = loadFile(pfilePath_fs.c_str());

    return LoadShaderData(name, vertexShaderString, fragmentShaderString);
}

void InitGLStates() {
  //    glShadeModel(GL_SMOOTH);
  //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //    glReadBuffer(GL_BACK);
  //    glDrawBuffer(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_TRUE);
  //    glDisable(GL_STENCIL_TEST);
  //    glStencilMask(0xFFFFFFFF);
  //    glStencilFunc(GL_EQUAL, 0x00000000, 0x00000001);
  //    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  //    glFrontFace(GL_CCW);
  ////    glCullFace(GL_BACK);
  ////    glEnable(GL_CULL_FACE);
  //    glDisable(GL_CULL_FACE);
  //    glClearColor(0.0, 0.0, 0.0, 0.0);
  glClearDepth(1.0);
  //    glClearStencil(0);
  //    glDisable(GL_BLEND);
  glEnable(GL_BLEND);

  // FIXME: if we use GL_ONE, font render may have color bg
  //    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //    glDisable(GL_ALPHA_TEST);
  //    glDisable(GL_DITHER);

  glActiveTexture(GL_TEXTURE0);
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

    LoadShader("Shader_Default", shaderDir + "light.vert", shaderDir + "light.frag");

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

        // must do this
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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
