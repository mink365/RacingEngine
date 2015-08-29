#include "Skybox.h"

#include "Texture/PVRTextureUtil.h"

#include "ShapeGenerater.h"

Skybox::Skybox()
{
    this->name = "SkyboxTest";
}

static EntityPtr box;
static EntityPtr skybox;

void Skybox::Init()
{
    auto file = FileSystem::getInstance().getFile("Textures/Cubemap/FishermansBastion_rgba8888.pvr");

    TexturePtr tex = Texture::create();
    tex->setFile(file);
    bool state = PVRTextureUtil::LoadFromPVR(*tex.get(), NULL, 0);

    LoadShader("cubemap", "Shaders/cubemap.vert", "Shaders/cubemap.frag");

    TextureParser::instance().addTextures("Textures/NormalMap", "png|jpg");
    auto texture = TextureManager::instance().getTexture("diffuse");

    {
        GeometryPtr geometry = ShapeGenerater::instance().CreateBox(30, 30, 30, 30, 30, 30);

        box = CreateMeshEntity();
        SetMeshData(box, geometry, tex, "cubemap");
    }

    {
        GeometryPtr geometry = ShapeGenerater::instance().CreateBox(900, 900, 900, 30, 30, 30);

        skybox = CreateMeshEntity();
        SetMeshData(skybox, geometry, tex, "cubemap");

        skybox->getComponent<RenderElement>()->getMaterial()->getRenderState().faceCullMode = FaceCullMode::Off;
    }

    rootNode->addChild(skybox->getNode());
    rootNode->addChild(box->getNode());

    this->camera->setDepthField(10, 1320);
    this->camera->setView(Vec3(0, 100, 0), Vec3(0, 3, 1), Vec3(0, 1, 0));
    this->camera->setQueueCullFunc([](int queue) {
        if (queue == RenderQueues::UI) {
            return false;
        }
        return true;
    });
}

static Quat quat;
static float r = 0;
void Skybox::Update()
{
    r += 0.02;

    box->getTransform()->setLocalRotation(quat.fromAngles(Vec3(0, 0, r)));

    float rad = r / 2.0;
    this->camera->setView(Vec3(0, 100, 0), Vec3(sin(rad), 100 + cos(rad), 0), Vec3(0, 0, 1));
}
