#include "ShadowTest.h"

#include "ShapeGenerater.h"
#include "GLCheckUtil.h"
#include "Scene/Light/DirectionalLight.h"
#include "Scene/Light/SpotLight.h"
#include "UI/Base/Sprite.h"
#include "Render/RenderTarget.h"
#include "Scene/SceneNode.h"

ShadowTest::ShadowTest()
{
}

static SceneNodePtr box;
static SceneNodePtr ground;
static SpritePtr sprite;

struct FboIDs {
    GLuint textureId;
    GLuint fboId;
    GLuint rboId;
};

static FboIDs ids;
FboIDs CreateRenderBuffer();
void SetupShadowMapShader();

void ShadowTest::Init()
{
    std::string shaderDir = "Shaders/";

    LoadShader("depth_rgba", shaderDir + "position_texture_color.vert",
                             shaderDir + "depth_rgba.frag");

    LoadShader("multi_texture", shaderDir + "position_texture_color.vert",
                             shaderDir + "multi_texture.frag");

    LoadShader("shadow_map", shaderDir + "shadow_map.vert",
                             shaderDir + "shadow_map.frag");

    TextureParser::getInstance().addTextures("Textures/NormalMap", "png|jpg");

    this->camera->setView(Vec3(0, -100, 260), Vec3(0, 0, 0), Vec3(0, 1, 0));

    auto texture = TextureManager::getInstance().getTexture("diffuse");

    MeshPtr groundMesh = ShapeGenerater::getInstance().CreateBox(300, texture);
    InitMeshInHardward(groundMesh, "shadow_map");

    ground = std::make_shared<SceneNode>();
    AddMeshToNode(ground, groundMesh);

    ground->setLocalTranslation(Vec3(0, 0, -200));

    rootNode->addChild(ground);
    ground->updateTransform();

    MeshPtr mesh = ShapeGenerater::getInstance().CreateBox(50, texture);
    InitMeshInHardward(mesh);

    box = std::make_shared<SceneNode>();
    AddMeshToNode(box, mesh);

    box->setLocalTranslation(Vec3(0, 0, 22));
    box->setLocalRotation(Quat().fromAngles(Vec3(50, 20, 0)));

    rootNode->addChild(box);
    box->updateTransform();

    // TODO: Dir Light/Camera can't see the model?.....

    // light
    re::LightPtr light = std::make_shared<SpotLight>();
    SceneManager::getInstance().addRootNode(light);
    SceneManager::getInstance().getRenderManager().addLight(light);

    light->setCastShadow(true);
    light->setLocalTranslation(Vec3(0, 0, 200));
    light->updateTransform();
    auto dirLight = std::dynamic_pointer_cast<SpotLight>(light);
    dirLight->shadow.shadowCameraNear = 10;
    dirLight->shadow.shadowCameraFar = 1000;
    dirLight->spotAngle = 50;
    dirLight->shadowCameraFov = 50;

    sprite = std::make_shared<Sprite>("diffuse.png");
    sprite->rebind();
    sprite->setPosition(Vec2(0, 0));

    auto scene = stage->pushTo("Scene1");
    auto window = scene->pushWindow("HelloWindow");
    window->addChild(sprite);

    SetupShadowMapShader();

    // what the fuck ?????? can't create renderbuffer here ? and will break the env?
//    ids = CreateRenderBuffer();
}

static re::Mat4 shadowMatrix;
static std::vector<re::Vec2> shadowMapSize;
static std::vector<float> shadowDarkness;
static std::vector<float> shadowBias;

// texture unit 2

void SetupShadowMapShader() {
    shadowMatrix.set(0.5, 0.0, 0.0, 0.5,
                     0.0, 0.5, 0.0, 0.5,
                     0.0, 0.0, 0.5, 0.5,
                     0.0, 0.0, 0.0, 1.0);

    re::Mat4 shadowCameraModelMatrix;
    shadowCameraModelMatrix.lookAt(Vec3(0, 0, 200),
                                   Vec3(0, 0, 0),
                                   Vec3(0, 1, 0));
    re::Mat4 shadowCameraModelInverseMatrix = shadowCameraModelMatrix.inverse();

    re::Mat4 shadowCameraProjMatrix;
    shadowCameraProjMatrix.setPerspective(50, 10, 1000);

    shadowMatrix *= shadowCameraProjMatrix;
    shadowMatrix *= shadowCameraModelInverseMatrix;

    shadowMapSize = {
        re::Vec2(512, 512),
        re::Vec2(512, 512),
    };

    shadowDarkness = {
        0.5, 0.5
    };

    shadowBias = {
        0.0001, 0.0001
    };

    Shader::ptr shader = ShaderManager::getInstance().getShader("shadow_map");

    shader->getUniform("shadowMatrix")->setData(shadowMatrix);
//    shader->getUniform("shadowMapSize")->setData(shadowMapSize[0]);
    shader->getUniform("shadowDarkness")->setData(shadowDarkness.data());
    shader->getUniform("shadowBias")->setData(shadowBias.data());

    shader = ShaderManager::getInstance().getShader("multi_texture");

//    glActiveTexture(GL_TEXTURE0 + 1);
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, 1);
//    glActiveTexture(GL_TEXTURE0);
}


static int TEXTURE_WIDTH = 256;
static int TEXTURE_HEIGHT = 256;
FboIDs CreateRenderBuffer() {
    glGetError();

//    glEnable(GL_TEXTURE_2D);

    GLuint textureId, fboId, rboId;

    // create a texture object
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);


    if(true)
    {
        // create a framebuffer object, you need to delete them when program exits.
        glGenFramebuffers(1, &fboId);
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);

        // create a renderbuffer object to store depth info
        // NOTE: A depth renderable image should be attached the FBO for depth test.
        // If we don't attach a depth renderable image to the FBO, then
        // the rendering output will be corrupted because of missing depth test.
        // If you also need stencil test for your rendering, then you must
        // attach additional image to the stencil attachement point, too.
        glGenRenderbuffers(1, &rboId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // attach a texture to FBO color attachement point
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

        // attach a renderbuffer to depth attachment point
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);

        //@@ disable color buffer if you don't attach any color buffer image,
        //@@ for example, rendering the depth buffer only to a texture.
        //@@ Otherwise, glCheckFramebufferStatus will not be complete.
        //glDrawBuffer(GL_NONE);
        //glReadBuffer(GL_NONE);

        // check FBO status
        printFramebufferInfo();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);

    return FboIDs{textureId, fboId, rboId};
}

void displayCB();
void RenderToFbo(GLuint fboId);
void DrawTexture(GLuint textureId);
static float playTime = 3;

static const float CAMERA_DISTANCE = 6;

static float rotateValue = 0;
static float screenWidth, screenHeight;
static float cameraDistance, cameraAngleX, cameraAngleY;
void ShadowTest::Update(float dt)
{
    screenWidth = 500;
    screenHeight = 600;

    rotateValue += 0.02;
    if (rotateValue > 360) {
        rotateValue = 0;
    }

    cameraDistance = 6;
    cameraAngleX = rotateValue * 2;
    cameraAngleY = rotateValue * 3;

    box->setLocalRotation(Quat().fromAngles(Vec3(0, 20, rotateValue)));

    auto mesh = std::dynamic_pointer_cast<re::Mesh>(sprite->getNodeAttribute());
    TextureUnitState::ptr unit = mesh->getMaterial()->getPass(0)->getTextureUnit(0);

    auto& renderManager = SceneManager::getInstance().getRenderManager();

    if (unit->getTextureFrameCount() <= 1 && renderManager.renderViewList.size() > 2) {
        auto view = renderManager.renderViewList[0];
        auto texture = view->renderTarget->getTexture();

        unit->addTextureFrame(texture);
        unit->setActiveTextureFrame(1);
    }

    if (renderManager.renderViewList.size() > 2) {
        auto view = renderManager.renderViewList[0];

        glUseProgram(0);

        glEnable(GL_TEXTURE_2D);
//        glEnable(GL_CULL_FACE);

//        RenderToFbo(view->renderTarget->framebuffer);

//        DrawTexture(view->renderTarget->getTexture()->getGlID());

        glActiveTexture(GL_TEXTURE0 + 1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, view->renderTarget->getTexture()->getGlID());
        glActiveTexture(GL_TEXTURE0);
    }

    renderManager.initDefaultRenderState();
}

void DrawTexture(GLuint textureId)
{
    glViewport(0, 0, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)(screenWidth)/screenHeight, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // tramsform camera
    glTranslatef(0, 0, -cameraDistance);
    glRotatef(cameraAngleX, 1, 0, 0);   // pitch
    glRotatef(cameraAngleY, 0, 1, 0);   // heading

//    // clear framebuffer
//    glClearColor(0, 0, 0, 0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glPushMatrix();

    // draw a cube with the dynamic texture
    DrawCube(textureId);

    glPopMatrix();
}


void RenderToFbo(GLuint fboId) {
    playTime += 0.01;

    // compute rotation angle
    const float ANGLE_SPEED = 90;   // degree/s
    float angle = ANGLE_SPEED * playTime;

    // render to texture //////////////////////////////////////////////////////

    // adjust viewport and projection matrix to texture dimension
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)(TEXTURE_WIDTH)/TEXTURE_HEIGHT, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);

    // camera transform
    glLoadIdentity();
    glTranslatef(0, 0, -CAMERA_DISTANCE);

    // with FBO
    // render directly to a texture
    bool fboUsed = true;
    if(fboUsed)
    {
        // set the rendering destination to FBO
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);

        // clear buffer
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw a rotating teapot at the origin
        glPushMatrix();
        glRotatef(angle*0.5f, 1, 0, 0);
        glRotatef(angle, 0, 1, 0);
        glRotatef(angle*0.7f, 0, 0, 1);
        glTranslatef(0, -1.575f, 0);
//        drawTeapot();

        DrawCube(2);

        glPopMatrix();

        // back to normal window-system-provided framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind

//        // trigger mipmaps generation explicitly
//        // NOTE: If GL_GENERATE_MIPMAP is set to GL_TRUE, then glCopyTexSubImage2D()
//        // triggers mipmap generation automatically. However, the texture attached
//        // onto a FBO should generate mipmaps manually via glGenerateMipmap().
//        glBindTexture(GL_TEXTURE_2D, ids.textureId);
//        glGenerateMipmap(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void displayCB()
{
    RenderToFbo(ids.fboId);
    DrawTexture(ids.textureId);
}
