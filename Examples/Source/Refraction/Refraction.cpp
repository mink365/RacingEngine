#include "Refraction.h"

#include "Texture/PVRTextureUtil.h"
#include "ShapeGenerater.h"

Refraction::Refraction()
{
    this->name = "Refraction";
}

static EntityPtr node;
void Refraction::Init()
{
    std::string shaderDir = "Shaders/";

    LoadShader("refraction", shaderDir + "refraction.vsh",
                             shaderDir + "refraction.fsh");

    TextureParser::instance().addTextures("Textures/NormalMap", "png|jpg");
    auto texture = TextureManager::instance().getTexture("diffuse");

    {
        GeometryPtr geometry = ShapeGenerater::instance().CreatePlane(300, 300, 30, 30);

        auto node = CreateMeshEntity();
        SetMeshData(node, geometry, texture);

        node->getComponent<RenderElement>()->getMaterial()->setQueueID(51);

        rootNode->addChild(node->getNode());

        node->getTransform()->setLocalTranslation(Vec3(0, 0, -70));
    }

    auto geometry = ShapeGenerater::instance().CreateTorus(70, 20, 30, 30);

    node = CreateMeshEntity();
    SetMeshData(node, geometry, texture, "refraction");
    node->getComponent<RenderElement>()->getMaterial()->setTexture("sTexture", texture);

    rootNode->addChild(node->getNode());
}

static float rotateValue = 0;
void Refraction::Update()
{
    this->setupShader();

    rotateValue += 0.02;
    if (rotateValue > 360) {
        rotateValue = 0;
    }

    node->getTransform()->setLocalRotation(Quat().fromAngles(Vec3(0, rotateValue, rotateValue)));
}

void Refraction::setupShader()
{
    Shader::ptr shader = ShaderManager::instance().GetResource("refraction");

    Vec3 lightPosition = Vec3(100, 100, 0);

    Vec3 cameraPosition = camera->getNode()->getTransform()->getWorldMatrix().getTranslation();
    Vec3 modelPosition = node->getTransform()->getWorldMatrix().getTranslation();
    Vec3 eyePosModel = cameraPosition - modelPosition;

    Vec3 lightPosModel = lightPosition - modelPosition;
    Vec3 lightDirModel = lightPosModel.normalize();

    shader->getUniform("lightDirModel")->setData(lightDirModel);
    shader->getUniform("eyePosModel")->setData(eyePosModel);
}
