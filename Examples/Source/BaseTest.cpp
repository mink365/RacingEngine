#include "BaseTest.h"
#include "FeatureTestsApp.h"

void InitMeshInHardward(MeshPtr mesh) {
    Geometry::ptr geometry = mesh->getGeometry();
    BufferObjectUtil::getInstance().loadGeometryToHardware(*(geometry.get()));

    Shader::ptr shader = ShaderManager::getInstance().getShader("Shader_PTC_TM");
    mesh->getMaterial()->setShder(shader);
}

void AddMeshToNode(SceneNodePtr node, MeshPtr mesh) {
    node->setNodeAttribute(mesh->clone());

    for (auto child : node->getChildren()) {
        AddMeshToNode(dynamic_pointer_cast<SceneNode>(child), mesh);
    }
}

BaseTest::BaseTest()
{
}

BaseTest::~BaseTest()
{

}

void BaseTest::init(FeatureTestsApp &app)
{
    this->camera = app.presCamera;
    this->rootNode = app.rootNode;
    this->stage = app.stage;

    this->Init();
}

void BaseTest::Init()
{

}

void BaseTest::Update(float dt)
{

}
