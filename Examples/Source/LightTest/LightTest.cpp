#include "LightTest.h"

#include "ShapeGenerater.h"
#include "Math/Vector.h"

#include "opengl.h"

LightTest::LightTest()
{
}

SceneNodePtr box = nullptr;

void LightTest::Init()
{
    std::string shaderDir = "Shaders/";

    LoadShader("Lighting", shaderDir + "lights_lambert.vert",
                             shaderDir + "lights_lambert.frag");

    TextureParser::getInstance().addTextures("Textures/NormalMap", "png|jpg");


    this->camera->setView(Vec3(0, 0, 260), Vec3(0, 0, 0), Vec3(0, 1, 0));

    auto texture = TextureManager::getInstance().getTexture("diffuse");

    MeshPtr mesh = ShapeGenerater::getInstance().CreateBox(50, texture);
//    InitMeshInHardward(mesh, "Shader_PTC");
    InitMeshInHardward(mesh, "Lighting");

    box = std::make_shared<SceneNode>();
    AddMeshToNode(box, mesh);

    box->setLocalTranslation(Vec3(0, 0, 52));
    box->setLocalRotation(Quat().fromAngles(Vec3(50, 20, 0)));

    rootNode->addChild(box);
    box->updateTransform();

}

Vec3 ambient(255/255.0, 255/255.0, 255/255.0);
Vec3 diffuse(255/255.0, 255/255.0, 255/255.0);
Vec3 emissive(15/255.0, 0/255.0, 0/255.0);
Vec3 ambientLightColor(0/255.0, 15/255.0, 0/255.0);

const int directionalLightCount = 2;

float list[directionalLightCount] = {1, 2};

Vec3 directionalLightColors[2] = {
    Vec3{1, 0, 0},
    Vec3{0, 0.4, 0}
};
Vec3 directionalLightDirections[directionalLightCount] = {
    Vec3{0, 1, 0},
    Vec3{0, 0, 1}
};

Mat3 normalMatrix;

float opacity = 1.0;

float rotateValue = 0;

void LightTest::Update(float dt)
{
    rotateValue += 0.04;
    if (rotateValue > 360) {
        rotateValue = 0;
    }

    box->setLocalRotation(Quat().fromAngles(Vec3(0, 20, rotateValue)));

    Shader::ptr shader = ShaderManager::getInstance().getShader("Lighting");

    shader->getUniform("opacity")->setData(&opacity);
    shader->getUniform("ambient")->setData(ambient);
    shader->getUniform("diffuse")->setData(diffuse);
    shader->getUniform("emissive")->setData(emissive);
    shader->getUniform("ambientLightColor")->setData(ambientLightColor);
    shader->getUniform("directionalLightColor")->setData(directionalLightColors[0]);
    shader->getUniform("directionalLightDirection")->setData(directionalLightDirections[0]);

    re::Mat4 modelMatrix = box->getWorldMatrix();
    normalMatrix[0].set(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
    normalMatrix[1].set(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
    normalMatrix[2].set(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);

    shader->getUniform("normalMatrix")->setData(normalMatrix);

//    Uniform* uniform = nullptr;

//    uniform = shader->getUniform("opacity");
//    glUniform1fv(uniform->location, uniform->nElements, &opacity);

//    uniform = shader->getUniform("directionalLightColor");
//    glUniform3fv(uniform->location, uniform->nElements, (GLfloat *)directionalLightColors);
//    uniform = shader->getUniform("directionalLightDirection");
//    glUniform3fv(uniform->location, uniform->nElements, (GLfloat *)directionalLightDirections);
}
