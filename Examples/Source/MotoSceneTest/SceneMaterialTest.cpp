#include "SceneMaterialTest.h"
#include "Material/Material.h"
#include "Material/MaterialManager.h"
#include "Loader/MaterialLoader.h"
#include "Resource/ModelLoader/fbx/FbxParser.h"
#include "Resource/ResourceManager.h"

SceneMaterialTest::SceneMaterialTest()
{
    this->name = "MaterialTest";
}

void InitNodeShader(SceneNodePtr& node) {
    MeshPtr mesh = dynamic_pointer_cast<Mesh>(node->getNodeAttribute());

    if (mesh) {
        InitMeshInHardward(mesh);
    }

    for (auto child : node->getChildren()) {
        auto childSceneNode = std::dynamic_pointer_cast<SceneNode>(child);
        InitNodeShader(childSceneNode);
    }
}

void SceneMaterialTest::Init()
{
    this->camera->setDepthField(10, 3000);
    this->camera->setView(Vec3(0, -1840, 557), Vec3(0, 30, 20), Vec3(0, 0, 1));

    TextureParser::getInstance().addTextures("Textures/Scenes/", "png|jpg");
    TextureManager::getInstance().loadTextures();

    FilePtr file = FileSystem::getInstance().openFile("Material/Scene/scene.material");
    MaterialLoader::getInstance().Load(file);

    FbxParser *parser = new FbxParser();
    file = FileSystem::getInstance().openFile("Model/scene.data");
    parser->parse(file);

    auto blockRootNode = parser->getNodes()[0];
    InitNodeShader(blockRootNode);

    rootNode->addChild(blockRootNode);

//    auto& manager = TexManager::getInstance();
//    manager.Hello();

//    auto tex = TextureManager::getInstance().getTexture("tttt");
//    manager.Register(tex);
}

static float s = 1;
static float dir = -1;
void SceneMaterialTest::Update(float dt)
{
    if (s < 0.1) {
        dir = 1;
    }

    if (s > 1) {
        dir = -1;
    }

    s += 1 / (60 * 7.0) * dir;

    this->camera->setView(Vec3(0, -1840 * s, 557 * s), Vec3(0, 30, 20), Vec3(0, 0, 1));
}
