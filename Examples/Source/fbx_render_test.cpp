
//In this example, we will use a few IBO/VBO and a few VAO.
//We will use glDrawRangeElements to render some shapes.
//We will use a couple of simple shaders. GL 3.3 shaders.
//We will create a 3.3 forward context with freeGLUT.
//
//We are using GLEW to get function pointers.
//
//As for the VBO, we will use an interleaved vertex format.
//Vertex, texcoords and normals.
//Vertex and color.
//
//As for the IBO, we will use 16 bit unsigned integers.
//
//http://freeglut.sourceforge.net
//http://glew.sourceforge.net

#include "fbx_render_test.h"

#include "Font/FreeTypeUtil.h"
#include "Font/FontManager.h"
#include "UI/Base/NinePatch.h"
#include "UI/Base/Sprite.h"
#include "UI/Base/Label.h"
#include "UI/Manager/UISceneManager.h"
#include "UI/Window.h"
#include "UI/Scene.h"
#include "UI/Widget/Button.h"
#include "Message/MessageManager.h"
#include "Render/RenderQueue.h"

#include "Platform/GameHub.h"
#include "Util/TypeUtil.h"
#include "TweenManager.h"
#include "Tween.h"
#include "Timeline.h"
#include "Util/PredefineTweenAccessor.h"

using namespace TweenEngine;

std::string resDir = "/home/jk/workspace/engines/RacingEngine/Examples/Resources/";

void update(long dt);

FBXTestApp::FBXTestApp()
{
    GameHub::getInstance().bindUpdateFunc(&update);
}

bool FBXTestApp::initEnvironment()
{
    Screen::getInstance().setDesignSize(614, 1024);

    Screen::getInstance().setRealFrameSize(this->view->getFrameSize());

    // TODO: filesystem

    return true;
}

void FBXTestApp::onEnterForeground()
{

}

void FBXTestApp::onExitForeground()
{

}

bool inited = false;
void update(long dt) {
    if (inited) {
        MessageManager::getInstance()->handleMessages();

        updateMatrix(false);
    } else {
        initResource();

        inited = true;
    }
}

// loadFile - loads text file into char* fname
// allocates memory - so need to delete after use
// size of file returned in fSize
std::string loadFile(const char *fname)
{
    std::ifstream file(fname);
    if(!file.is_open())
    {
        std::cout << "Unable to open file " << fname << std::endl;
        exit(1);
    }

    std::stringstream fileData;
    fileData << file.rdbuf();
    file.close();

    return fileData.str();
}

void InitGLStates()
{
//    glShadeModel(GL_SMOOTH);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//    glReadBuffer(GL_BACK);
//    glDrawBuffer(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(TRUE);
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

    //FIXME: if we use GL_ONE, font render may have color bg
//    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//    glDisable(GL_ALPHA_TEST);
//    glDisable(GL_DITHER);

    glActiveTexture(GL_TEXTURE0);
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
int LoadShaderData(std::string name, std::string vs, std::string fs) {
    Shader::ptr shader = Shader::create();
    shader->setName(name);

    shader->setVertexSource(vs);
    shader->setFragmentSource(fs);

    ShaderUtil::getInstance().compileShader(shader.get());

    CheckShaderLinkError(shader->getProgram());

    if (name == "Shader_Default") {
        setLightShaderAttribute(shader);
    } else if (name == "Shader_PTC") {
        setPTCShaderAttribute(shader);
    } else if (name == "Shader_Font") {
        setPTCShaderAttribute(shader);
    }

//    shader.getUniform("model")->setData((float*)model);
//    shader->getUniform("view")->setData((float*)camera.getViewMatrix());
//    shader->getUniform("projection")->setData((float*)camera.getProjectionMatrix());

    ShaderManager::getInstance().registerShader(shader);

    return 1;
}

int LoadShader(const std::string& name, const std::string& pfilePath_vs, const std::string& pfilePath_fs)
{
    // load shaders & get length of each
    std::string vertexShaderString = loadFile(pfilePath_vs.c_str());
    std::string fragmentShaderString = loadFile(pfilePath_fs.c_str());

    return LoadShaderData(name, vertexShaderString, fragmentShaderString);
}

MeshPtr createBox(float side, Texture::ptr texture = nullptr) {
    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->init();

    float coords[] = {
        -side/2.0f, -side/2.0f, side/2.0f, //v0
        side/2.0f, -side/2.0f, side/2.0f, 	//v1
        -side/2.0f, side/2.0f, side/2.0f, 	//v2
        side/2.0f, side/2.0f, side/2.0f, 	//v3

        side/2.0f, -side/2.0f, side/2.0f, 	//...
        side/2.0f, -side/2.0f, -side/2.0f,
        side/2.0f, side/2.0f, side/2.0f,
        side/2.0f, side/2.0f, -side/2.0f,

        side/2.0f, -side/2.0f, -side/2.0f,
        -side/2.0f, -side/2.0f, -side/2.0f,
        side/2.0f, side/2.0f, -side/2.0f,
        -side/2.0f, side/2.0f, -side/2.0f,

        -side/2.0f, -side/2.0f, -side/2.0f,
        -side/2.0f, -side/2.0f, side/2.0f,
        -side/2.0f, side/2.0f, -side/2.0f,
        -side/2.0f, side/2.0f, side/2.0f,

        -side/2.0f, -side/2.0f, -side/2.0f,
        side/2.0f, -side/2.0f, -side/2.0f,
        -side/2.0f, -side/2.0f, side/2.0f,
        side/2.0f, -side/2.0f, side/2.0f,

        -side/2.0f, side/2.0f, side/2.0f,
        side/2.0f, side/2.0f, side/2.0f,
        -side/2.0f, side/2.0f, -side/2.0f,
        side/2.0f, side/2.0f, -side/2.0f,
            };

    float textureCood[] = {
        //Mapping coordinates for the vertices
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };

    float normals[] = {
                        //Normals
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,

                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,

                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,

                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,

                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,

                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f
                                            };

    short indices[] = {
        // Faces definition
        0, 1, 3, 0, 3, 2, 		// Face front
        4, 5, 7, 4, 7, 6, 		// Face right
        8, 9, 11, 8, 11, 10, 	// ...
        12, 13, 15, 12, 15, 14,
        16, 17, 19, 16, 19, 18,
        20, 21, 23, 20, 23, 22,
    };

    for (int i = 0; i < 24; ++i) {
      Vertex v;
      v.xyz.set(coords[i * 3 + 0], coords[i * 3 + 1], coords[i * 3 + 2]);

      v.uv.set(textureCood[i * 2 + 0], textureCood[i * 2 + 1]);

      v.normal.set(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);

      mesh->getGeometry()->addVertex(v);
    }

    for (int i = 0; i < 12; ++i) {
        Face f(indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]);

        mesh->getGeometry()->addFace(f);
    }

    TextureUnitState &unit = mesh->getMaterial()->getTexture();
    unit.setUVstate(0, 0, 1, 1, 0);

    if (texture == nullptr) {
        texture = TextureManager::getInstance().getTexture("girl");
    }
    unit.addTextureFrame(texture);

    return mesh;
}

const static int BLOCK_LENGTH = 157;
const static int BLOCK_COUNT = 5;
std::vector<SceneNodePtr> blocks;
SceneNodePtr black_box;
SceneNodePtr motoRoot;
SceneNodePtr box;
shared_ptr<Node2d> widget;
SkeletonControllerPtr manController;
float y = 0, z = 0, zV = 1, rotation = 0;

CameraPtr presCamera;
TweenEngine::TweenManager* tweenManager = new TweenEngine::TweenManager();

// blocks切换的次数记录
int block_index = 0;
void updateMatrix(bool isAnim) {
    tweenManager->update(1 / 50.0);

    if (isAnim) {
        rotation += 40.0 / (1000 / 60);

        y += 2;

        if (z > 5) {
            zV = -0.6f;
        } else if (z < -5) {
            zV = 0.6f;
        }
        z += zV;
    }

    if (y > block_index * BLOCK_LENGTH) {
        // 交换block

        SceneNodePtr first = blocks[0];

        Vec3 old = first->getLocalTranslation();
        first->setLocalTranslation(Vec3(0, old.y + BLOCK_LENGTH * BLOCK_COUNT, 0));

        blocks.erase(blocks.begin());
        blocks.push_back(first);

        block_index += 1;
    }

//    camera.setViewport(600, 600);
//    camera.setDepthField(10, 1320);
//    camera.setView(Vec3(0, y + 30, 50), Vec3(0, -240 + y, 57 + z), Vec3(0, 1, 0));
//    camera.updateTransform();

    black_box->setLocalTranslation(Vec3(0, y - 100, 0));

//    Mat4 rotationM;
//    rotationM.setRotationY(rotation * DEG_TO_RAD);

//    float scale = 1.0f / 200;
//    model.setScaling(scale, scale, scale);

//    model *= rotationM;
////    model.setTranslation(0.0f, 0.0f, -4);
///

    rotation += 0.04;
    if (rotation > 360) {
        rotation = 0;
    }

//    widget->setRotation(rotation * 20);

    Quat quat;
    quat.fromAngles(Vec3(0, 0, rotation));
    motoRoot->setLocalRotation(quat);

    manController->update();
    MeshPtr mesh = manController->getMesh();

    Geometry::ptr geometry = mesh->getGeometry();
    BufferObjectUtil::getInstance().updateGeometryToHardware(*(geometry.get()));

    mesh->getNode()->setLocalRotation(quat);

    box->setLocalRotation(quat);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
}

void AddMeshToNode(SceneNodePtr node, MeshPtr mesh) {
    node->setNodeAttribute(mesh->clone());

    for (auto child : node->getChildren()) {
        AddMeshToNode(dynamic_pointer_cast<SceneNode>(child), mesh);
    }
}

void InitMeshInHardward(MeshPtr mesh) {
    Geometry::ptr geometry = mesh->getGeometry();
    BufferObjectUtil::getInstance().loadGeometryToHardware(*(geometry.get()));

    Shader::ptr shader = ShaderManager::getInstance().getShader("Shader_Default");
    mesh->getMaterial()->setShder(shader);
}

void initResource()
{
    InitGLStates();

    const Screen& screen = Screen::getInstance();

    presCamera = std::make_shared<Camera>();
    presCamera->setViewport(screen.getRealSize().width, screen.getRealSize().height);
    presCamera->setDepthField(10, 1320);
    presCamera->setView(Vec3(0, 30, 20), Vec3(0, -340, 57), Vec3(0, 0, 1));
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
    searchPath.rootDir = resDir.c_str();
    searchPath.type = FileType::Permanent;
    FileSystem::getInstance().addSearchPath(searchPath);

    std::string shaderDir = resDir + "Shaders/";
    std::string assertDir = resDir + "Model/PAD/";

    LoadShader("Shader_Default",
                        shaderDir + "light.vert",
                        shaderDir + "light.frag");

    LoadShader("Shader_PTC",
                        shaderDir + "position_texture_color.vert",
                        shaderDir + "position_texture_color.frag");

    LoadShader("Shader_Font",
                      shaderDir + "v3f-t2f-c4f.vert",
                      shaderDir + "v3f-t2f-c4f.frag");

    TextureManager::getInstance().setImageLoader(new ImageLoader());

    TextureParser::getInstance().addTextures("Model/PAD/", "png|jpg");

//    TextureManager::getInstance().loadTextures();

    FbxParser *parser = new FbxParser();

    FilePtr file = FileSystem::getInstance().openFile((assertDir + "black.data"));
    parser->parse(file);
    SceneNodePtr black = parser->getNodes()[0];

    file = FileSystem::getInstance().openFile((assertDir + "wall.data"));
    parser->parse(file);

    SceneNodePtr wall = parser->getNodes()[0];
    SceneNodePtr floor = parser->getNodes()[1];

    assertDir = resDir + "Model/Moto/";

    TextureParser::getInstance().addTextures("Model/Moto/", "png|jpg");
    TextureParser::getInstance().addTextures("Model/Man/", "png|jpg");

    TextureManager::getInstance().loadTextures();

    file = FileSystem::getInstance().openFile((assertDir + "new_group_moto03.data"));
    parser->parse(file);
    SceneNodePtr shadow = parser->getNodes()[0];
    SceneNodePtr moto = parser->getNodes()[1];

    std::vector<SceneNodePtr> meshs;
    meshs.push_back(wall);
    meshs.push_back(floor);
    meshs.push_back(black);
    meshs.push_back(shadow);
    meshs.push_back(moto);

    for (SceneNodePtr node : meshs) {
        MeshPtr mesh = dynamic_pointer_cast<Mesh>(node->getNodeAttribute());

        InitMeshInHardward(mesh);
    }

    for (int i = 0; i < BLOCK_COUNT; ++i) {
        SceneNodePtr block = std::make_shared<SceneNode>();

        auto wall_copy = wall->clone();

        auto floor_copy = floor->clone();

        block->addChild(wall_copy);
        block->addChild(floor_copy);

        block->setLocalTranslation(Vec3(0, 0 + BLOCK_LENGTH * (i - 3), 0));

        SceneManager::getInstance().addRootNode(block);

        blocks.push_back(block);
    }

    motoRoot = std::make_shared<SceneNode>();
    motoRoot->addChild(shadow);
    motoRoot->addChild(moto);
    motoRoot->setLocalTranslation(Vec3(0, 0, 12));

    // TODO:
//    moto->getMaterial()->getRenderState().setDepthTest(true);

    SceneManager::getInstance().addRootNode(motoRoot);
    black_box = black;

    assertDir = resDir + "Model/Man/";
    parser->parse(assertDir + "group_girl.data");
    manController = parser->getSkeletonController("girl");

    AnimationTrackPtr track = manController->getAnimation()->getCurrAnimationTrack();
    Long beginTime = track->getKeyFrame(2)->getTime();
    Long endTime = track->getKeyFrame(12)->getTime();
    manController->getAnimation()->addAnimationStack(std::make_shared<AnimationStack>(beginTime, endTime));
    manController->getAnimation()->setAnimationStackIndex(0);
    manController->getAnimation()->setAnimationLoop(true);
    manController->getAnimation()->setAnimationPower(1.0);
    manController->getAnimation()->setIsUseAnimationStack(true);

//    manController->setDefaultFrame(1);

    InitMeshInHardward(manController->getMesh());
    SceneManager::getInstance().addRootNode(manController->getMeshNode());

    MeshPtr mesh = createBox(2);
    InitMeshInHardward(mesh);

    box = std::make_shared<SceneNode>();
    AddMeshToNode(box, mesh);

//    BoneNodePtr rootBone = manController->getSkeleton()->getRootBone();
//    SceneNodePtr node = rootBone;

//    AddMeshToNode(node, mesh);

//    box->setLocalTranslation(Vec3(0, 0, 52));
//    SceneManager::getInstance().addRootNode(node);

    string hello("test");

    TestFont();

    TestUI();
}

Font::ptr font;
void TestFont()
{
    auto fontFile = resDir + "Fonts/ObelixPro.ttf";

    font = std::make_shared<Font>(128, fontFile.c_str());
    font->setName("default");
    FontManager::getInstance().registerFont(font);

    auto atlas = TextureAtlas::create();
    atlas->init(512, 512, 1);

    FreeTypeUtil::LoadGlyphs(atlas, font, L"Hello People我xtb");

    atlas->upload();

    MeshPtr mesh = createBox(50, atlas->getTexture());
    InitMeshInHardward(mesh);

    box = std::make_shared<SceneNode>();
    AddMeshToNode(box, mesh);

    box->setLocalTranslation(Vec3(0, 0, 52));
    SceneManager::getInstance().addRootNode(box);
}

template<typename T, typename S>
bool IsBaseOf(T t, S t2) {
    bool result = is_base_of<T, S>();

//    auto identity = [](decltype(t) t){ return t;};
//    std::cout << identity(t) << std::endl;

    return result;
}

void func(std::shared_ptr<void> obj) {

}

void TestFunction() {
    Node* p = nullptr;
    Window* w = nullptr;

    std::shared_ptr<void> pp = std::make_shared<Node>();

    std::function<void(std::shared_ptr<void>)> ff = &(func);

    auto delat = CreateIt<Window>(w, [](Window* obj, int type, int key){
        int t = type;
        int k = key;
    });

    std::map<int, MyFunctionDecl> map;
    map[3] = delat;

    (*map[3])(3, 7);
}

void TestUI()
{
    SceneNodePtr ui = std::make_shared<SceneNode>();

    TextureParser::getInstance().addTextures("UI/", "png");
    TextureManager::getInstance().loadTextures();

    SpritePtr sprite = std::make_shared<Sprite>("store_icon_coin.png");
    sprite->rebind();
    sprite->setPosition(Vec2(300, 300 + 200));

    NinePatchPtr patch = std::make_shared<NinePatch>("tab_press.png");
    patch->setStrethPadding(20, 20, 20, 20);
    patch->setContentSize(Size(200, 100));
    patch->rebind();
    patch->setPosition(Vec2(300, 300 + 80));
    patch->setAnchorPoint(Vec2(0.5, 0.5));

    ui->addChild(sprite);
    ui->addChild(patch);

    LabelPtr label = std::make_shared<Label>();
    label->init(font);
    label->setText("xH<size=50>e<color=FF0000FF>l</color>l</size>o <color=00FFFF>xxo</color>tbo");

    LabelPtr label2 = std::make_shared<Label>();
    label2->init(font);
    label2->setText("xHtbo xx");

    label2->setPosition(Vec2(300, 300 + 40));

    ui->addChild(label);
    ui->addChild(label2);

    auto button = CreateView<ImageButton>();
    button->initView("rate.png", "rate_press.png", "rate.png");

//    Quat quat;
//    quat.fromAngles(Vec3(PI / 2.0f, 0, 0));
//    ui->setLocalRotation(quat);
//    float scale = 0.3f;
//    ui->setLocalScaling(Vec3(1.0f, 1.0f, 1.0f) * scale);
//    ui->setLocalTranslation(Vec3(-30, -100, 0));
//    SceneManager::getInstance().addRootNode(ui);

    auto uiManager = CreateView<ui::UISceneManager>();
    SceneManager::getInstance().addRootNode(uiManager);
    MessageManager::getInstance()->addHandler(uiManager.get());

    std::shared_ptr<ISceneFactory> factory = std::make_shared<SceneFactory>();
    uiManager->setSceneFactory(factory);

    auto scene = uiManager->pushTo("Scene1");
    auto window = scene->pushWindow("HelloWindow");
    window->addChild(sprite);
    window->addChild(patch);
    window->addChild(label);
    window->addChild(label2);
    window->addChild(button);

//    window->setPosition(Vec2(300, 300));

    LayoutUtil::layoutParentCenter(button);

    label->setContentSize(Size(200, 50));
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setRotation(300);
    label->setPosition(window->getContentSize().width/2.0, window->getContentSize().height / 2.0);


    PredefineTweenAccessor::registerAccessor();

    Tween::to(label, FlatNodeAccessor::POSITION_X, 1).target(180).delay(1.0f).repeat(5, 0.5).start(*tweenManager);

    Timeline::createSequence()
        // First, set all objects to their initial positions
        .push(Tween::set(sprite, FlatNodeAccessor::ROTATION).target(10))
        .push(Tween::set(label2, FlatNodeAccessor::ROTATION).target(20))
        .push(Tween::set(patch, FlatNodeAccessor::ROTATION).target(30))

        // Wait 1s
        .pushPause(1.0f)

        // Move the objects around, one after the other
        .push(Tween::to(sprite, FlatNodeAccessor::ROTATION, 1.0).target(90))
        .push(Tween::to(label2, FlatNodeAccessor::ROTATION, 2.0).target(90))
        .push(Tween::to(patch, FlatNodeAccessor::ROTATION, 1.0).target(90))

        // Then, move the objects around at the same time
        .beginParallel()
            .push(Tween::to(sprite, FlatNodeAccessor::ROTATION, 1.0).target(180))
            .push(Tween::to(label2, FlatNodeAccessor::ROTATION, 1.0).target(160))
            .push(Tween::to(patch, FlatNodeAccessor::ROTATION, 1.0).target(150))
        .end()

        // And repeat the whole sequence 2 times
        // with a 0.5s pause between each iteration
        .repeatYoyo(5, 0.5f)

        // Let's go!
        .start(*tweenManager);

    widget = patch;

    Node2d* p = new Node2d();
    Node* b = label.get();
    Node* c = dynamic_cast<Label*>(b);

    const type_info& t = typeid(Node);
    bool rv = t.__do_catch(&t, (void**)&b, 0);
//    t.__do_upcast();

    bool r = std::is_base_of<Node, Node2d>();
    bool v = __is_base_of(Node, Node2d);

    bool result = IsBaseOf(window, label);

    bool test = IsBaseOf(window.get(), typeid(Node), typeid(TextureAtlas));

    return;
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
