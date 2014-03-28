#ifndef FBX_RENDER_TEST_H
#define FBX_RENDER_TEST_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <shader/ShaderUtil.h>
#include <camera/Camera.h>
#include <render/bufferobject/BufferObjectUtil.h>
#include <scene/Mesh.h>
#include <tools/ModelLoader/fbx/FbxParser.h>
#include <texture/TextureManager.h>
#include <scene/SceneManager.h>

#include "opengl.h"

using namespace std;

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

    Attribute *normalAttr = shader.getAttribute("aNormal");
    normalAttr->setType(ATTR_FORMAT_FLOAT);
    normalAttr->setSize(3);
    normalAttr->setStride(sizeof(Vertex));
    normalAttr->setOffset((5) * 4);

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


    LoadShader(vertexShaderString, fragmentShaderString);
}

namespace Test {
void printFloatArray(std::string head, float *v, int count, int split) {

    std::ostringstream oss;

    oss << head << "Array: {" << "\n";
    for (int i = 0; i < count / split; ++i) {
        oss << "V[";
        for (int j = 0; j < split; ++j) {
            oss << v[i * split + j] << ", ";
        }

        oss << "]" << "\n";
    }

    std::cout << oss.str() << std::endl;
}
}

const static int BLOCK_LENGTH = 157;
const static int BLOCK_COUNT = 5;
std::vector<SceneNodePtr> blocks;
SceneNodePtr black_box;
SceneNodePtr motoRoot;
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

        reVec3 old = first->getLocalTranslation();
        first->setLocalTranslation(reVec3(0, old.y + BLOCK_LENGTH * BLOCK_COUNT, 0));

        blocks.erase(blocks.begin());
        blocks.push_back(first);

        block_index += 1;
    }

//    camera.setViewport(600, 600);
    camera.setDepthField(10, 1320);
    camera.setView(reVec3(0, y + 30, 50), reVec3(0, -240 + y, 57 + z), reVec3(0, 1, 0));

    black_box->setLocalTranslation(reVec3(0, y - 100, 0));

//    reMat4 rotationM;
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

    reQuat quat;
    quat.fromAngles(reVec3(0, 0, rotation));
    motoRoot->setLocalRotation(quat);
}

void registerTexture(string path) {
    Texture *texture = new Texture();
    texture->setPath(path);
    TextureManager::getInstance().registerTexture(*texture);
}

void initResource()
{
    InitGLStates();

    std::string resDir = "/home/kou/workspace/droidhen/kourui/RacingEngineC++/examples/";
//    std::string resDir = "/sdcard/book/racing/";
    std::string shaderDir = resDir + "shaders/";
    std::string assertDir = resDir + "model/PAD/";

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

    MeshPtr black = dynamic_pointer_cast<Mesh>(parser->getNodes()[0]);

    filestr.open ((assertDir + "wall.data").c_str(), ios::binary);
    parser->parseStream(&filestr);
    filestr.close();

    MeshPtr wall = dynamic_pointer_cast<Mesh>(parser->getNodes()[0]);
    MeshPtr floor = dynamic_pointer_cast<Mesh>(parser->getNodes()[1]);

    assertDir = resDir + "model/Moto/";

    registerTexture(assertDir + "moto03.jpg");
    registerTexture(assertDir + "moto_yingzi.png");
    registerTexture(assertDir + "moto.jpg");
    registerTexture(assertDir + "huoyan_01.png");
    registerTexture(assertDir + "huoyan_02.png");
    registerTexture(assertDir + "huoyan_03.png");
    registerTexture(assertDir + "qiliu.png");

    TextureManager::getInstance().loadTextures();

    filestr.open((assertDir + "new_group_moto03.data").c_str(), ios::binary);
    parser->parseStream(&filestr);
    filestr.close();
    MeshPtr shadow = dynamic_pointer_cast<Mesh>(parser->getNodes()[0]);
    MeshPtr moto = dynamic_pointer_cast<Mesh>(parser->getNodes()[1]);

    std::vector<MeshPtr> meshs;
    meshs.push_back(wall);
    meshs.push_back(floor);
    meshs.push_back(black);
    meshs.push_back(shadow);
    meshs.push_back(moto);

    for (auto node : meshs) {
        Geometry *geometry = &(node->getGeometry());
        BufferObjectUtil::getInstance().loadGeometryToHardware(*geometry);

        node->getMaterial().setShder(&shader);
    }

//    SceneNode *block = new SceneNode();
//    block->addChild(wall);
//    block->addChild(floor);
//    SceneManager::getInstance().addRootNode(*block);

    for (int i = 0; i < BLOCK_COUNT; ++i) {
        SceneNodePtr block = std::make_shared<SceneNode>();

        auto wall_copy = std::make_shared<Mesh>();
        *wall_copy = *wall;

        auto floor_copy = std::make_shared<Mesh>();
        *floor_copy = *floor;


        block->addChild(wall_copy);
        block->addChild(floor_copy);

        block->setLocalTranslation(reVec3(0, 0 + BLOCK_LENGTH * (i - 3), 0));

        SceneManager::getInstance().addRootNode(block);

        blocks.push_back(block);
    }

    motoRoot = std::make_shared<SceneNode>();
    motoRoot->addChild(shadow);
    motoRoot->addChild(moto);
    motoRoot->setLocalTranslation(reVec3(0, 0, 12));
    moto->getMaterial().getRenderState().setDepthTest(true);

//    black->setVisible(false);
//    SceneManager::getInstance().addRootNode(black);
    SceneManager::getInstance().addRootNode(motoRoot);
    black_box = black;
}

#endif // FBX_RENDER_TEST_H
