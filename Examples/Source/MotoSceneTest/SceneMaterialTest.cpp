#include "SceneMaterialTest.h"
#include "Material/Material.h"
#include "Material/MaterialManager.h"
#include "Loader/MaterialLoader.h"
#include "Resource/ModelLoader/fbx/FbxParser.h"

SceneMaterialTest::SceneMaterialTest()
{
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
    TextureParser::getInstance().addTextures("Textures/Scenes/", "png|jpg");
    TextureManager::getInstance().loadTextures();

    FilePtr file = FileSystem::getInstance().openFile("Material/Scene/scene.material");
    MaterialLoader::getInstance().Load(file);

    FbxParser *parser = new FbxParser();
    file = FileSystem::getInstance().openFile("Model/scene.data");
    parser->parse(file);

    // TODO:
    auto blockRootNode = parser->getNodes()[0];
    InitNodeShader(blockRootNode);

    rootNode->addChild(blockRootNode);
}

void SceneMaterialTest::Update(float dt)
{

}
