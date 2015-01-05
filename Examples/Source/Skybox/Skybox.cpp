#include "Skybox.h"

#include "Texture/PVRTextureUtil.h"

#include "ShapeGenerater.h"

Skybox::Skybox()
{
    this->name = "SkyboxTest";
}

static SceneNodePtr box;
static SceneNodePtr skybox;

void Skybox::Init()
{
    auto file = FileSystem::getInstance().getFile("Textures/Cubemap/FishermansBastion_rgba8888.pvr");

    TexturePtr tex = Texture::create();
    tex->setFile(file);
    bool state = PVRTextureUtil::LoadFromPVR(*tex.get(), NULL, 0);

    LoadShader("cubemap", "Shaders/cubemap.vert", "Shaders/cubemap.frag");

    TextureParser::getInstance().addTextures("Textures/NormalMap", "png|jpg");
    auto texture = TextureManager::getInstance().getTexture("diffuse");

    {
        GeometryPtr geometry = ShapeGenerater::getInstance().CreateBox(30, 30, 30, 30, 30, 30);
        MeshPtr mesh = ShapeGenerater::getInstance().CreateMesh(geometry, tex);
        InitMeshInHardward(mesh, "cubemap");

        box = std::make_shared<SceneNode>();
        AddMeshToNode(box, mesh);
    }

    {
        GeometryPtr geometry = ShapeGenerater::getInstance().CreateBox(900, 900, 900, 30, 30, 30);
        MeshPtr mesh = ShapeGenerater::getInstance().CreateMesh(geometry, tex);
        InitMeshInHardward(mesh, "cubemap");

        mesh->getMaterial()->getRenderState().faceCullMode = FaceCullMode::Off;

        skybox = std::make_shared<SceneNode>();
        AddMeshToNode(skybox, mesh);
    }

    rootNode->addChild(skybox);
    rootNode->addChild(box);

    this->camera->setDepthField(10, 1320);
    this->camera->setView(Vec3(0, 100, 0), Vec3(0, 3, 1), Vec3(0, 1, 0));
    this->camera->setQueueCullFunc([](int queue) {
        if (queue == RENDER_QUEUE_UI) {
            return false;
        }
        return true;
    });
}

static Quat quat;
static float r = 0;
void Skybox::Update(float dt)
{
    r += 0.02;

    box->getTransform()->setLocalRotation(quat.fromAngles(Vec3(0, 0, r)));

    float rad = r / 2.0;
    this->camera->setView(Vec3(0, 100, 0), Vec3(sin(rad), 100 + cos(rad), 0), Vec3(0, 0, 1));
}
