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

    this->camera->setView(Vec3(100, 0, 300), Vec3(0, 0, 0), Vec3(0, 1, 0));

    auto texture = TextureManager::getInstance().getTexture("diffuse");

    MeshPtr groundMesh = ShapeGenerater::getInstance().CreateBox(300, texture);
    InitMeshInHardward(groundMesh, "shadow_map");
    groundMesh->getMaterial()->setQueueID(51);

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
    light->setLocalTranslation(Vec3(0, -200, 300));
    light->updateTransform();
    auto dirLight = std::dynamic_pointer_cast<SpotLight>(light);
    dirLight->shadow.shadowCameraNear = 10;
    dirLight->shadow.shadowCameraFar = 500;
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

static std::vector<re::Mat4> shadowMatrixs;
static std::vector<re::Vec2> shadowMapSize;
static std::vector<float> shadowDarkness;
static std::vector<float> shadowBias;

// texture unit 2

void SetupShadowMapShader() {
    re::Mat4 biasMatrix(0.5, 0.0, 0.0, 0.5,
                        0.0, 0.5, 0.0, 0.5,
                        0.0, 0.0, 0.5, 0.5,
                        0.0, 0.0, 0.0, 1.0);

    re::Mat4 shadowCameraViewMatrix;
    shadowCameraViewMatrix.lookAt(Vec3(0, -200, 300),
                                   Vec3(0, 0, 0),
                                   Vec3(0, 1, 0));

    re::Mat4 shadowCameraProjMatrix;
    shadowCameraProjMatrix.setPerspective(50, 10, 500);

    auto shadowMatrix = biasMatrix * shadowCameraProjMatrix * shadowCameraViewMatrix;
    shadowMatrixs = {
        shadowMatrix,
        shadowMatrix,
    };

    re::Vec3 v(0, 0, -600);
    re::Vec3 r = shadowMatrix * v;

    re::Mat4 m;
//    m.setTranslation(0,0,200);
    m.fromRTS(Quat().fromAngles(Vec3(79,0,0)), Vec3(2, 1, 3), Vec3(0,0,200));

    re::Vec3 v1(0, 0, 190);
    re::Vec3 r1 = shadowCameraProjMatrix * v1;

    re::Vec4 v2(0, 0, 190, 1.0);
    re::Vec4 r2 = shadowCameraProjMatrix * v2;

    shadowMapSize = {
        re::Vec2(512, 512),
        re::Vec2(512, 512),
    };

    shadowDarkness = {
        0.8, 0.8
    };

    shadowBias = {
        0.0001, 0.0001
    };

    Shader::ptr shader = ShaderManager::getInstance().getShader("shadow_map");

    shader->getUniform("shadowMatrix")->setData(shadowMatrixs[0]);
//    shader->getUniform("shadowMapSize")->setData(shadowMapSize[0]);
    shader->getUniform("shadowDarkness")->setData(shadowDarkness.data());
    shader->getUniform("shadowBias")->setData(shadowBias.data());
}

static float rotateValue = 0;
void ShadowTest::Update(float dt)
{
    rotateValue += 0.02;
    if (rotateValue > 360) {
        rotateValue = 0;
    }

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

        glActiveTexture(GL_TEXTURE0 + 1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, view->renderTarget->getTexture()->getGlID());
        glActiveTexture(GL_TEXTURE0);

        glActiveTexture(GL_TEXTURE0 + 2);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, view->renderTarget->getTexture()->getGlID());
        glActiveTexture(GL_TEXTURE0);
    }

    // TODO: clear func?
    renderManager.initDefaultRenderState();
}
