#include "RenderToTextureTest.h"

#include "ShapeGenerater.h"
#include "Render/Renderer/Renderer.h"
#include "Render/RenderTarget.h"

#include "Math/Vector.h"

RenderToTextureTest::RenderToTextureTest()
{
    this->name = "RenderToTextureTest";
}

Renderer* renderer;
RenderManager* renderManager;
RenderTargetPtr renderTarget;

SceneNodePtr cylinderNode, boxNode;

void RenderToTextureTest::Init()
{
    camera->setDepthField(10, 1320);
    camera->setView(Vec3(0, -340, 57), Vec3(0, 30, 20), Vec3(0, 0, 1));
    camera->setQueueCullFunc([](int queue) {
        if (queue == 51) {
            return true;
        }
        return false;
    });

    renderManager = &(SceneManager::getInstance().getRenderManager());
    renderer = &(renderManager->getRenderer());

    TextureParser::getInstance().addTextures("Textures/NormalMap", "png|jpg");

    auto texture = TextureManager::getInstance().getTexture("diffuse");

    GeometryPtr geometry = nullptr;
    MeshPtr mesh = nullptr;

    geometry = ShapeGenerater::getInstance().CreateCylinder(50, 50, 100, 10, 10);
    mesh = ShapeGenerater::getInstance().CreateMesh(geometry, texture);
    InitMeshInHardward(mesh);
    mesh->getMaterial()->setQueueID(52);

    cylinderNode = std::make_shared<SceneNode>();
    AddMeshToNode(cylinderNode, mesh);

    rootNode->addChild(cylinderNode);

    // create a render target
    renderTarget = std::make_shared<RenderTarget>();
    renderTarget->setHasDepthBuffer(true);
    renderTarget->setHasStencilBuffer(false);

    renderer->setupRenderTarget(*renderTarget);

    auto size = renderTarget->getSize();

    auto presCamera = std::make_shared<Camera>();
    presCamera->setViewport(size.width, size.height);
    presCamera->setDepthField(10, 1320);
    presCamera->setView(Vec3(0, -340, 57), Vec3(0, 30, 20), Vec3(0, 0, 1));
    presCamera->setQueueCullFunc([](int queue) {
        if (queue == 52) {
            return true;
        }
        return false;
    });
    presCamera->setRenderTarget(renderTarget);
    presCamera->setClearColor(Color(169.0/255, 74.0/255, 174.0/255, 1));

    SceneManager::getInstance().getRenderManager().addCamera(presCamera);
    SceneManager::getInstance().addRootNode(presCamera);

    // create the normal geometry
    geometry = ShapeGenerater::getInstance().CreateBox(100, 100, 100, 10, 10, 10);
    mesh = ShapeGenerater::getInstance().CreateMesh(geometry, renderTarget->getTexture());
    InitMeshInHardward(mesh);
    mesh->getMaterial()->setQueueID(51);

    boxNode = std::make_shared<SceneNode>();
    AddMeshToNode(boxNode, mesh);

    rootNode->addChild(boxNode);
}

static float rotateValue = 0;
void RenderToTextureTest::Update(float dt)
{
    MeshPtr mesh = std::dynamic_pointer_cast<Mesh>(cylinderNode->getNodeAttribute());
    TextureUnitState::ptr state = mesh->getMaterial()->getPass(0)->getTextureUnit(0);
    Texture::ptr tex = state->getTexture();

    rotateValue += 0.02;
    if (rotateValue > 360) {
        rotateValue = 0;
    }

    cylinderNode->setLocalRotation(Quat().fromAngles(Vec3(0, rotateValue, rotateValue)));
}