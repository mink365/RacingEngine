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

EntityPtr cylinderNode, boxNode;

void RenderToTextureTest::Init()
{
    camera->setDepthField(10, 1320);
    camera->setView(Vec3(0, -340, 57), Vec3(0, 30, 20), Vec3(0, 0, 1));
    camera->setQueueCullFunc([](int queue) {
        if (queue == 50) {
            return true;
        }
        return false;
    });

    renderManager = &(SceneManager::instance().getRenderManager());
    renderer = &(renderManager->getRenderer());

    TextureParser::instance().addTextures("Textures/NormalMap", "png|jpg");

    auto texture = TextureManager::instance().getTexture("diffuse");

    GeometryPtr geometry = nullptr;
    MeshPtr mesh = nullptr;

    geometry = ShapeGenerater::instance().CreateCylinder(50, 50, 100, 10, 10);

    cylinderNode = CreateMeshEntity();
    SetMeshData(cylinderNode, geometry, texture);
    cylinderNode->getComponent<RenderElement>()->getMaterial()->setQueueID(52);

    rootNode->addChild(cylinderNode->getNode());

    // create a render target
    renderTarget = Create<RenderTarget>();
    renderTarget->setHasDepthBuffer(true);
    renderTarget->setHasStencilBuffer(false);

    renderer->setupRenderTarget(*renderTarget);

    auto size = renderTarget->getSize();

    auto presCamera = CreateNode<Camera>();
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

    SceneManager::instance().getRenderManager().addCamera(presCamera);
    SceneManager::instance().addRootNode(presCamera->getNode());

    // create the normal geometry
    geometry = ShapeGenerater::instance().CreateBox(100, 10, 100, 20, 20, 20);

    boxNode = CreateMeshEntity();
    SetMeshData(boxNode, geometry, renderTarget->getTexture());

    rootNode->addChild(boxNode->getNode());
}

static float rotateValue = 0;
void RenderToTextureTest::Update()
{
    rotateValue += 0.02;
    if (rotateValue > 360) {
        rotateValue = 0;
    }

    cylinderNode->getTransform()->setLocalRotation(Quat().fromAngles(Vec3(0, rotateValue, rotateValue)));
}
