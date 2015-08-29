#include "Reflection.h"

#include "Texture/PVRTextureUtil.h"
#include "ShapeGenerater.h"

Reflection::Reflection()
{
    this->name = "Reflection";
}

static EntityPtr node;
void Reflection::Init()
{
    std::string shaderDir = "Shaders/";

    LoadShader("reflection", shaderDir + "reflection.vsh",
                             shaderDir + "reflection.fsh");

    auto file = FileSystem::getInstance().getFile("Textures/Cubemap/FishermansBastion_rgba8888.pvr");
    TexturePtr cubeTexture = Texture::create();
    cubeTexture->setFile(file);
    bool state = PVRTextureUtil::LoadFromPVR(*cubeTexture.get(), NULL, 0);

    TextureParser::instance().addTextures("Textures/NormalMap", "png|jpg");
    auto texture = TextureManager::instance().getTexture("diffuse");

    auto geometry = ShapeGenerater::instance().CreateTorus(70, 20, 30, 30);

    node = CreateMeshEntity();
    SetMeshData(node, geometry, texture, "reflection");

    rootNode->addChild(node->getNode());

    auto material = node->getComponent<RenderElement>()->getMaterial();
    material->setQueueID(51);
    material->setTexture("s2DMap", texture);
    material->setTexture("sCubeMap", cubeTexture);
}

static float rotateValue = 0;
void Reflection::Update()
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
    Shader::ptr shader = ShaderManager::instance().GetResource("reflection");

    Vec3 cameraPosition = camera->getEntity()->getTransform()->getWorldMatrix().getTranslation();
    Vec3 modelPosition = node->getTransform()->getWorldMatrix().getTranslation();
    Vec3 eyePosModel = cameraPosition - modelPosition;
    shader->getUniform("eyePosModel")->setData(eyePosModel);

    float v = 1.0;
    shader->getUniform("bCubeReflection")->setData(&v);
}
