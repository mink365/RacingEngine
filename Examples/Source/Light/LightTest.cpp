#include "LightTest.h"

#include "ShapeGenerater.h"
#include "Math/Vector.h"

#include "opengl.h"

LightTest::LightTest()
{
    this->name = "LightTest";
}

NodePtr box = nullptr;

const bool PHONG_LIGHT = true;
string CURRENT_SHADER_NAME;
void InitLightData();

void LightTest::Init()
{
    std::string shaderDir = "Shaders/";

    LoadShader("lights_lambert", shaderDir + "lights_lambert.vert",
                             shaderDir + "lights_lambert.frag");

    LoadShader("lights_phong", shaderDir + "lights_phong.vert",
                             shaderDir + "lights_phong.frag");

    TextureParser::getInstance().addTextures("Textures/NormalMap", "png|jpg");

    if (PHONG_LIGHT) {
        CURRENT_SHADER_NAME = "lights_phong";
    } else {
        CURRENT_SHADER_NAME = "lights_lambert";
    }

    this->camera->setDepthField(10, 1320);
    this->camera->setView(Vec3(0, 0, 260), Vec3(0, 0, 0), Vec3(0, 1, 0));
    this->camera->setQueueCullFunc([](int queue) {
        if (queue == RENDER_QUEUE_UI) {
            return false;
        }
        return true;
    });

    auto texture = TextureManager::getInstance().getTexture("diffuse");

    MeshPtr mesh = ShapeGenerater::getInstance().CreateBox(50, texture);
//    InitMeshInHardward(mesh, "Shader_PTC");
    InitMeshInHardward(mesh, CURRENT_SHADER_NAME);

    box = CreateNode();
    AddMeshToNode(box, mesh);

    box->getTransform()->setLocalTranslation(Vec3(0, 0, 52));
    box->getTransform()->setLocalRotation(Quat().fromAngles(Vec3(50, 20, 0)));

    rootNode->addChild(box);
    box->updateTransform();

    InitLightData();
}

Vec3 ambient(255/255.0, 255/255.0, 255/255.0);
Vec3 diffuse(255/255.0, 255/255.0, 255/255.0);
Vec3 emissive(15/255.0, 0/255.0, 0/255.0);
Vec3 ambientLightColor(0/255.0, 15/255.0, 30/255.0);

Mat3 normalMatrix;

float opacity = 1.0;
float shininess = 10.0;
Vec3 specular(0.7, 0.1, 0.2);

const int DIR_LIGHT_COUNT = 0;
const int HEMI_LIGHT_COUNT = 0;
const int POINT_LIGHT_COUNT = 2;
const int PORT_LIGHT_COUNT = 0;

std::vector<Vec3> directionalLightColors;
std::vector<Vec3> directionalLightDirections;

std::vector<Vec3> hemisphereLightSkyColor;
std::vector<Vec3> hemisphereLightGroundColor;
std::vector<Vec3> hemisphereLightDirection;

std::vector<Vec3> pointLightColor;
std::vector<Vec3> pointLightPosition;
std::vector<float> pointLightDistance;

std::vector<Vec3> spotLightColor;
std::vector<Vec3> spotLightPosition;
std::vector<Vec3> spotLightDirection;
std::vector<float> spotLightDistance;
std::vector<float> spotLightAngleCos;
std::vector<float> spotLightExponent;


float rotateValue = 0;

void InitLightData() {

    // ----------------- Dir light ----------------

    directionalLightColors = {
        Vec3{1, 0, 0},
        Vec3{0, 0.4, 0},
    };

    directionalLightDirections = {
        Vec3{0, 1, 0},
        Vec3{0, 0, 1}
    };

    // ------------------ Hemi light ----------------

    hemisphereLightSkyColor = {
        Vec3{0, 1, 0},
        Vec3{0, 0, 1}
    };

    hemisphereLightGroundColor = {
        Vec3{0, 1, 0},
        Vec3{0, 0, 1}
    };

    hemisphereLightDirection = {
        Vec3{0, 1, 0},
        Vec3{0, 0, 1}
    };

    // ------------------ Point light -----------------
    pointLightColor = {
        Vec3{3, 0, 4},
        Vec3{0, 0.4, 0},
    };

    pointLightPosition = {
        Vec3{10, 100, 80},
        Vec3{0, 0, 170},
    };

    pointLightDistance = {
        100, 100,
    };

    // ------------------- Spot light -----------------

    spotLightColor = {
        Vec3{0, 1, 0},
        Vec3{0, 0, 1}
    };

    spotLightPosition = {
        Vec3{0, 0, 100},
        Vec3{0, 100, 1}
    };

    spotLightDirection = {
        Vec3{0, 0, 1},
        Vec3{0, -1, 0},
    };

    spotLightDistance = {
        50, 50,
    };

    spotLightAngleCos = {
        30, 50,
    };

    spotLightExponent = {
        10, 10
    };

}

void LightTest::Update(float dt)
{
    rotateValue += 0.04;
    if (rotateValue > 360) {
        rotateValue = 0;
    }

    box->getTransform()->setLocalRotation(Quat().fromAngles(Vec3(0, 20, rotateValue)));

    Shader::ptr shader = ShaderManager::getInstance().getShader(CURRENT_SHADER_NAME);
    if (!PHONG_LIGHT) {
    } else {
        shader->getUniform("shininess")->setData(&shininess);
        shader->getUniform("specular")->setData(specular);
    }
    shader->getUniform("opacity")->setData(&opacity);

    shader->getUniform("ambient")->setData(ambient);
    shader->getUniform("diffuse")->setData(diffuse);
    shader->getUniform("emissive")->setData(emissive);
    shader->getUniform("ambientLightColor")->setData(ambientLightColor);

    if (DIR_LIGHT_COUNT > 0) {
        shader->getUniform("directionalLightColor")->setData(directionalLightColors[0]);
        shader->getUniform("directionalLightDirection")->setData(directionalLightDirections[0]);
    }

    if (HEMI_LIGHT_COUNT > 0) {
        shader->getUniform("hemisphereLightSkyColor")->setData(directionalLightColors[0]);
        shader->getUniform("hemisphereLightGroundColor")->setData(directionalLightDirections[0]);
        shader->getUniform("hemisphereLightDirection")->setData(directionalLightDirections[0]);
    }

    if (POINT_LIGHT_COUNT > 0) {
        shader->getUniform("pointLightColor")->setData(pointLightColor[0]);
        shader->getUniform("pointLightPosition")->setData(pointLightPosition[0]);
        shader->getUniform("pointLightDistance")->setData(pointLightDistance.data());
    }

    if (PORT_LIGHT_COUNT > 0) {
        shader->getUniform("spotLightColor")->setData(spotLightColor[0]);
        shader->getUniform("spotLightPosition")->setData(spotLightPosition[0]);
        shader->getUniform("spotLightDirection")->setData(spotLightDirection[0]);
        shader->getUniform("spotLightDistance")->setData(spotLightDistance.data());
        shader->getUniform("spotLightAngleCos")->setData(spotLightAngleCos.data());
        shader->getUniform("spotLightExponent")->setData(spotLightExponent.data());
    }

    re::Mat4 modelMatrix = box->getTransform()->getWorldMatrix();
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
