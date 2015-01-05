#include "Reflection.h"

#include "Texture/PVRTextureUtil.h"
#include "ShapeGenerater.h"

Reflection::Reflection()
{
    this->name = "Reflection";
}

static SceneNodePtr node;
void Reflection::Init()
{
    std::string shaderDir = "Shaders/";

    LoadShader("reflection", shaderDir + "reflection.vsh",
                             shaderDir + "reflection.fsh");

    auto file = FileSystem::getInstance().getFile("Textures/Cubemap/FishermansBastion_rgba8888.pvr");
    TexturePtr cubeTexture = Texture::create();
    cubeTexture->setFile(file);
    bool state = PVRTextureUtil::LoadFromPVR(*cubeTexture.get(), NULL, 0);

    TextureParser::getInstance().addTextures("Textures/NormalMap", "png|jpg");
    auto texture = TextureManager::getInstance().getTexture("diffuse");

    auto geometry = ShapeGenerater::getInstance().CreateTorus(70, 20, 30, 30);
    MeshPtr mesh = ShapeGenerater::getInstance().CreateMesh(geometry, texture);
    InitMeshInHardward(mesh, "reflection");
//    InitMeshInHardward(mesh);
    mesh->getMaterial()->setQueueID(51);

    node = std::make_shared<SceneNode>();
    AddMeshToNode(node, mesh);

    rootNode->addChild(node);

    auto unit = TextureUnitState::create();
    mesh->getMaterial()->getPass(0)->addTextureUnit(unit);
    unit->setTexture(cubeTexture);
}

static float rotateValue = 0;
void Reflection::Update(float dt)
{
    this->setupShader();

    rotateValue += 0.02;
    if (rotateValue > 360) {
        rotateValue = 0;
    }

    node->getTransform()->setLocalRotation(Quat().fromAngles(Vec3(0, rotateValue, rotateValue)));
}

void Reflection::setupShader()
{
    Shader::ptr shader = ShaderManager::getInstance().getShader("reflection");

    Vec3 cameraPosition = camera->getTransform()->getWorldMatrix().getTranslation();
    Vec3 modelPosition = node->getTransform()->getWorldMatrix().getTranslation();
    Vec3 eyePosModel = cameraPosition - modelPosition;
    shader->getUniform("eyePosModel")->setData(eyePosModel);

    float v = 1.0;
    shader->getUniform("bCubeReflection")->setData(&v);
}
