#include "ShadowTest.h"

#include "ShapeGenerater.h"
#include "Scene/Light/DirectionalLight.h"
#include "Scene/Light/SpotLight.h"
#include "UI/Base/Sprite.h"
#include "Render/RenderTarget.h"
#include "Scene/SceneNode.h"
#include "opengl.h"

ShadowTest::ShadowTest()
{
    this->name = "ShadowTest";
}

static SceneNodePtr box;
static SceneNodePtr ground;
static SpritePtr sprite;

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

    LoadShader("simple_shadow_map", shaderDir + "simple_shadow_map.vert",
                             shaderDir + "simple_shadow_map.frag");

    TextureParser::getInstance().addTextures("Textures/NormalMap", "png|jpg");

    this->camera->setDepthField(10, 1320);
    this->camera->setView(Vec3(100, 0, 300), Vec3(0, 0, 0), Vec3(0, 1, 0));
    this->camera->setQueueCullFunc([](int queue) {
        if (queue == RENDER_QUEUE_UI) {
            return false;
        }
        return true;
    });

    auto texture = TextureManager::getInstance().getTexture("diffuse");

//    MeshPtr groundMesh = ShapeGenerater::getInstance().CreateBox(300, texture);
    GeometryPtr geometry = nullptr;
    geometry = ShapeGenerater::getInstance().CreatePlane(200, 200, 30, 30);
//     geometry = ShapeGenerater::getInstance().CreateRing(50, 200);
    MeshPtr groundMesh = ShapeGenerater::getInstance().CreateMesh(geometry, texture);
    InitMeshInHardward(groundMesh, "shadow_map");
    groundMesh->getMaterial()->setQueueID(51);

    ground = std::make_shared<SceneNode>();
    AddMeshToNode(ground, groundMesh);

    // TODO: if the plane z > 100, shadow will be cull ?
    ground->getTransform()->setLocalTranslation(Vec3(0, 0, 0));

    rootNode->addChild(ground);
    ground->updateTransform();

    MeshPtr mesh = ShapeGenerater::getInstance().CreateBox(50, texture);
    InitMeshInHardward(mesh);

    box = std::make_shared<SceneNode>();
    AddMeshToNode(box, mesh);

    box->getTransform()->setLocalTranslation(Vec3(0, 0, 22));
    box->getTransform()->setLocalRotation(Quat().fromAngles(Vec3(50, 20, 0)));

    rootNode->addChild(box);
    box->updateTransform();

    // TODO: Dir Light/Camera can't see the model?.....

    // light
    re::LightPtr light = std::make_shared<SpotLight>();
    rootNode->addChild(std::dynamic_pointer_cast<SceneNode>(light->getNode()));
    SceneManager::getInstance().getRenderManager().addLight(light);

    light->setCastShadow(true);
    light->getNode()->getTransform()->setLocalTranslation(Vec3(0, -200, 300));
    light->getNode()->updateTransform();
    auto dirLight = std::dynamic_pointer_cast<SpotLight>(light);
    dirLight->shadow.shadowCameraNear = 10;
    dirLight->shadow.shadowCameraFar = 500;
    dirLight->spotAngle = 50;
    dirLight->shadowCameraFov = 50;

    sprite = CreateView<Sprite>("diffuse.png");
    sprite->rebind();
    sprite->getTransform2D()->setScale(Vec2(1, 1));
    sprite->getTransform2D()->setPosition(Vec2(0, 0));

    auto scene = stage->getLastLayer();
    auto window = scene->pushWindow("HelloWindow");
    window->addChild(sprite);

    LayoutUtil::LayoutToParent(sprite, AlignType::RIGHT_BOTTOM, AlignType::RIGHT_BOTTOM);

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
    glClearColor(1.0, 1.0, 1.0, 1.0);

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

    shadowMapSize = {
        re::Vec2(512, 512),
        re::Vec2(512, 512),
    };

    shadowDarkness = {
        0.8, 0.8
    };

    shadowBias = {
        0.000032, 0.000032
    };

    Shader::ptr shader = ShaderManager::getInstance().getShader("shadow_map");

    shader->getUniform("shadowMatrix")->setData(shadowMatrixs[0]);
//    shader->getUniform("shadowMapSize")->setData(shadowMapSize[0]);
    shader->getUniform("shadowDarkness")->setData(shadowDarkness.data());
    shader->getUniform("shadowBias")->setData(shadowBias.data());

    shader = ShaderManager::getInstance().getShader("depth_rgba");
//    shader->getUniform("shadowMatrix")->setData(shadowMatrixs[0]);
}

static float rotateValue = 0;
void ShadowTest::Update(float dt)
{
    rotateValue += 0.02;
    if (rotateValue > 360) {
        rotateValue = 0;
    }

    box->getTransform()->setLocalRotation(Quat().fromAngles(Vec3(0, rotateValue, rotateValue)));

    auto spriteMesh = sprite->getComponent<Mesh>();
    auto groundMesh = ground->getComponent<Mesh>();

    auto& renderManager = SceneManager::getInstance().getRenderManager();

    if (renderManager.renderViewList.size() > 2) {
        auto view = renderManager.renderViewList[0];
        auto texture = view->renderTarget->getTexture();

        TextureUnitState::ptr unit = spriteMesh->getMaterial()->getPass(0)->getTextureUnit(0);
        unit->setTexture(texture);

        if (groundMesh->getMaterial()->getPass(0)->getTextureUnitCount() < 2) {
            groundMesh->getMaterial()->getPass(0)->addTextureUnit(TextureUnitState::create());
            groundMesh->getMaterial()->getPass(0)->addTextureUnit(TextureUnitState::create());

            groundMesh->getMaterial()->getPass(0)->getTextureUnit(1)->setTexture(texture);
            groundMesh->getMaterial()->getPass(0)->getTextureUnit(2)->setTexture(texture);
        }
    }

//    // TODO: clear func?
    renderManager.initDefaultRenderState();
}
