#include "Refraction.h"

#include "Texture/PVRTextureUtil.h"
#include "ShapeGenerater.h"

Refraction::Refraction()
{
    this->name = "Refraction";
}

static NodePtr node;
void Refraction::Init()
{
    std::string shaderDir = "Shaders/";

    LoadShader("refraction", shaderDir + "refraction.vsh",
                             shaderDir + "refraction.fsh");

    TextureParser::getInstance().addTextures("Textures/NormalMap", "png|jpg");
    auto texture = TextureManager::getInstance().getTexture("diffuse");


    {
        GeometryPtr geometry = ShapeGenerater::getInstance().CreatePlane(300, 300, 30, 30);
        MeshPtr mesh = ShapeGenerater::getInstance().CreateMesh(geometry, texture);
        InitMeshInHardward(mesh);
        mesh->getMaterial()->setQueueID(51);

        auto node = CreateNode();
        AddMeshToNode(node, mesh);

        rootNode->addChild(node);

        node->getTransform()->setLocalTranslation(Vec3(0, 0, -70));
    }
    auto geometry = ShapeGenerater::getInstance().CreateTorus(70, 20, 30, 30);
    MeshPtr mesh = ShapeGenerater::getInstance().CreateMesh(geometry, texture);
    InitMeshInHardward(mesh, "refraction");
//    InitMeshInHardward(mesh);
    mesh->getMaterial()->setQueueID(51);

    node = CreateNode();
    AddMeshToNode(node, mesh);

    rootNode->addChild(node);
}

static float rotateValue = 0;
void Refraction::Update(float dt)
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
    Shader::ptr shader = ShaderManager::getInstance().getShader("refraction");

    Vec3 lightPosition = Vec3(100, 100, 0);

    Vec3 cameraPosition = camera->getNode()->getTransform()->getWorldMatrix().getTranslation();
    Vec3 modelPosition = node->getTransform()->getWorldMatrix().getTranslation();
    Vec3 eyePosModel = cameraPosition - modelPosition;

    Vec3 lightPosModel = lightPosition - modelPosition;
    Vec3 lightDirModel = lightPosModel.normalize();

    shader->getUniform("lightDirModel")->setData(lightDirModel);
    shader->getUniform("eyePosModel")->setData(eyePosModel);
}
