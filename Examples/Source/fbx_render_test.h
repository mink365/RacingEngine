#ifndef FBX_RENDER_TEST_H
#define FBX_RENDER_TEST_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader/ShaderUtil.h"
#include "Camera/Camera.h"
#include "Render/BufferObject/BufferObjectUtil.h"
#include "Scene/Mesh.h"
#include "Tools/ModelLoader/fbx/FbxParser.h"
#include "Texture/TextureManager.h"
#include "Scene/SceneManager.h"
#include "Animation/SkeletonController.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/BoneNode.h"

#include "opengl.h"

using namespace std;
using namespace re;

Shader shader;

Camera camera;

#define TRUE true
#define FALSE false

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
////    glDisable(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//    glDisable(GL_ALPHA_TEST);
//    glDisable(GL_DITHER);
    glActiveTexture(GL_TEXTURE0);
}

int LoadShader(std::string vs, std::string fs) {
    shader.setVertexSource(vs);
    shader.setFragmentSource(fs);

    ShaderUtil::getInstance().compileShader(&shader);

    GLint IsLinked;
    glGetProgramiv(shader.getProgram(), GL_LINK_STATUS, (GLint *)&IsLinked);
    if(IsLinked==FALSE)
    {
        LOG_E("Failed to link shader.");

        GLint maxLength;
        glGetProgramiv(shader.getProgram(), GL_INFO_LOG_LENGTH, &maxLength);
        if(maxLength > 0)
        {
            char *pLinkInfoLog = new char[maxLength];
            glGetProgramInfoLog(shader.getProgram(), maxLength, &maxLength, pLinkInfoLog);
            LOG_E("shader log: %s\n", pLinkInfoLog);

            delete [] pLinkInfoLog;
        }

        return -1;
    }

    // set vertex attribute
    Attribute *vertAttr = shader.getAttribute("aPosition");
    vertAttr->setType(ATTR_FORMAT_FLOAT);
    vertAttr->setSize(3);
    vertAttr->setStride(sizeof(Vertex));
    vertAttr->setOffset(0);

    Attribute *uvAttr = shader.getAttribute("aTexCoord");
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

//    shader.getUniform("model")->setData((float*)model);
    shader.getUniform("view")->setData((float*)camera.getViewMatrix());
    shader.getUniform("projection")->setData((float*)camera.getProjectionMatrix());

    return 1;               //Success
}

int LoadShader(const char *pfilePath_vs, const char *pfilePath_fs)
{
    // load shaders & get length of each
    std::string vertexShaderString = loadFile(pfilePath_vs);
    std::string fragmentShaderString = loadFile(pfilePath_fs);

    return LoadShader(vertexShaderString, fragmentShaderString);
}

MeshPtr createBox(float side) {
    MeshPtr mesh = std::make_shared<Mesh>();

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

      mesh->getGeometry().addVertex(v);
    }

    for (int i = 0; i < 12; ++i) {
        Face f(indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]);

        mesh->getGeometry().addFace(f);
    }

    TextureUnitState &unit = mesh->getMaterial().getTexture();
    unit.setUVstate(0, 0, 1, 1, 0);

    Texture &tex = TextureManager::getInstance().getTexture("girl");
    unit.addTextureFrame(&tex);

    return mesh;
}

const static int BLOCK_LENGTH = 157;
const static int BLOCK_COUNT = 5;
std::vector<SceneNodePtr> blocks;
SceneNodePtr black_box;
SceneNodePtr motoRoot;
SceneNodePtr box;
SkeletonControllerPtr manController;
float y = 0, z = 0, zV = 1, rotation = 0;

// blocks切换的次数记录
int block_index = 0;
void updateMatrix(bool isAnim) {
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
    camera.setDepthField(10, 1320);
    camera.setView(Vec3(0, y + 30, 50), Vec3(0, -240 + y, 57 + z), Vec3(0, 1, 0));

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

    Quat quat;
    quat.fromAngles(Vec3(0, 0, rotation));
    motoRoot->setLocalRotation(quat);

    manController->update();
    MeshPtr mesh = manController->getMesh();

    Geometry *geometry = &(mesh->getGeometry());
    BufferObjectUtil::getInstance().updateGeometryToHardware(*geometry);
    mesh->getMaterial().setShder(&shader);

    mesh->getNode()->setLocalRotation(quat);

    box->setLocalRotation(quat);
}

void registerTexture(string path) {
    Texture *texture = new Texture();
    texture->setPath(path);
    TextureManager::getInstance().registerTexture(*texture);
}

void AddMeshToNode(SceneNodePtr node, MeshPtr mesh) {
    node->setNodeAttribute(mesh->clone());

    for (auto child : node->getChildren()) {
        AddMeshToNode(dynamic_pointer_cast<SceneNode>(child), mesh);
    }
}

void InitMeshInHardward(MeshPtr mesh) {
    Geometry& geometry = mesh->getGeometry();
    BufferObjectUtil::getInstance().loadGeometryToHardware(geometry);
    mesh->getMaterial().setShder(&shader);
}

void initResource()
{
    InitGLStates();

    std::string resDir = "/home/jk/workspace/engines/RacingEngine/Examples/Resources/";
//    std::string resDir = "/sdcard/book/racing/";
    std::string shaderDir = resDir + "Shaders/";
    std::string assertDir = resDir + "Model/PAD/";

    if(LoadShader((shaderDir + "light.vert").c_str(), (shaderDir + "light.frag").c_str())==-1)
    {
        exit(1);
    }

    TextureManager::getInstance().setImageLoader(new ImageLoader());

    Texture *texture = new Texture();
    texture->setPath(assertDir + "floor.jpg");
    TextureManager::getInstance().registerTexture(*texture);

    texture = new Texture();
    texture->setPath(assertDir + "wall.jpg");
    TextureManager::getInstance().registerTexture(*texture);

    texture = new Texture();
    texture->setPath(assertDir + "black.png");
    TextureManager::getInstance().registerTexture(*texture);

//    TextureManager::getInstance().loadTextures();

    FbxParser *parser = new FbxParser();

    ifstream filestr;
    // 要读入整个文件，必须采用二进制打开
    filestr.open ((assertDir + "black.data").c_str(), ios::binary);

    filestr.seekg(0, filestr.end);
    int length = filestr.tellg();
    filestr.seekg(0, filestr.beg);

    char *buffer = new char[length];

    filestr.read(buffer, length);
    filestr.close();

    parser->parseData((void *)buffer, length);

    delete[] buffer;

    SceneNodePtr black = parser->getNodes()[0];

    filestr.open ((assertDir + "wall.data").c_str(), ios::binary);
    parser->parseStream(&filestr);
    filestr.close();

    SceneNodePtr wall = parser->getNodes()[0];
    SceneNodePtr floor = parser->getNodes()[1];

    assertDir = resDir + "Model/Moto/";

    registerTexture(assertDir + "moto03.jpg");
    registerTexture(assertDir + "moto_yingzi.png");
    registerTexture(assertDir + "moto.jpg");
    registerTexture(assertDir + "huoyan_01.png");
    registerTexture(assertDir + "huoyan_02.png");
    registerTexture(assertDir + "huoyan_03.png");
    registerTexture(assertDir + "qiliu.png");
    registerTexture(resDir + "Model/Man/girl.jpg");

    TextureManager::getInstance().loadTextures();

    filestr.open((assertDir + "new_group_moto03.data").c_str(), ios::binary);
    parser->parseStream(&filestr);
    filestr.close();
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
//    moto->getMaterial().getRenderState().setDepthTest(true);

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
}

#endif // FBX_RENDER_TEST_H
