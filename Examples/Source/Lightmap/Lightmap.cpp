#include "Lightmap.h"

#include "Texture/PVRTextureUtil.h"
#include "ShapeGenerater.h"

Lightmap::Lightmap()
{
    this->name = "Lightmap";
}

static NodePtr node;
void Lightmap::Init()
{
    this->camera->setDepthField(10, 1320);
    this->camera->setView(Vec3(0, 0, 150), Vec3(0, 0, 0), Vec3(0, 1, 0));
//    this->camera->setFieldOfView();
    this->camera->setQueueCullFunc([](int queue) {
        if (queue == RENDER_QUEUE_UI) {
            return false;
        }
        return true;
    });

    std::string shaderDir = "Shaders/";

    LoadShader("lightmap", shaderDir + "lightmap.vsh",
                             shaderDir + "lightmap.fsh");

    TextureParser::getInstance().addTextures("Textures/Lightmap", "png|jpg");
    auto base = TextureManager::getInstance().getTexture("tex_base");
    auto reflection = TextureManager::getInstance().getTexture("reflection");
    auto shadow = TextureManager::getInstance().getTexture("shadow");

    auto geometry = ShapeGenerater::getInstance().CreateTorus(30, 10, 30, 30);

    node = CreateMeshNode();
    SetMeshData(node, geometry, base, "lightmap");
    auto material = node->getComponent<Material>();

    rootNode->addChild(node);

    material->setQueueID(51);
    material->setTexture("sBasetex", base);
    material->setTexture("sReflect", reflection);
    material->setTexture("sShadow", shadow);
}

static float rotateValue = 0;
void Lightmap::Update(float dt)
{
    this->setupShader();

    rotateValue += 0.02;
    if (rotateValue > 360) {
        rotateValue = 0;
    }

    node->getTransform()->setLocalRotation(Quat().fromAngles(Vec3(0, rotateValue, rotateValue)));
}

void Lightmap::setupShader()
{
    Shader::ptr shader = ShaderManager::getInstance().getShader("lightmap");

    re::Mat4 modelM = node->getTransform()->getWorldMatrix();
    re::Mat4 modelViewM = camera->getViewMatrix() * modelM;

    re::Vec4 lightDirModel = modelM.inverse() * Vec4(1, 1, 1, 0);
    re::Vec4 eyePosModel = modelViewM.inverse() * Vec4(0, 0, 0, 1);

    shader->getUniform("LightDirModel")->setData(lightDirModel);
    shader->getUniform("EyePosModel")->setData(eyePosModel);

    // light map projection matrix
    Vec3 vLightFrom = Vec3( 85, -85, 3000);
    Vec3 vLightTo = Vec3(0, 0, -25);
    Vec3 vLightUp = Vec3(0, 1, 0);

    re::Mat4 viewMatrix;
    viewMatrix.lookAt(vLightFrom, vLightTo, vLightUp);

    re::Mat4 projMatrix;
    projMatrix.setPerspective(PI/2, 1, 2);

    re::Mat4 shadowViewPorj = projMatrix * viewMatrix;

    re::Mat4 ShadowProj = shadowViewPorj * node->getTransform()->getWorldMatrix();
    shader->getUniform("ShadowProj")->setData(ShadowProj);
}
