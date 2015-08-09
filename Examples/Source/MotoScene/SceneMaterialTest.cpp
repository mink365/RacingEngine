#include "SceneMaterialTest.h"
#include "Material/Material.h"
#include "Loader/MaterialLoader.h"
#include "Resource/ModelLoader/fbx/FbxParser.h"
#include "Resource/ResourceManager.h"
#include "UI/Base/Sprite.h"
#include "UI/Base/Label.h"
#include "UI/Manager/UIManager.h"
#include "UI/Widget/Button.h"
#include "UI/Layout/LayoutUtil.h"

extern std::shared_ptr<TextureAtlas> CreateDefaultFont();

SceneMaterialTest::SceneMaterialTest()
{
    this->name = "MaterialTest";
}

void InitNodeShader(NodePtr& node) {
    auto func = [](NodePtr& node) {
        MeshPtr mesh = node->getComponent<Mesh>();

        if (mesh) {
            InitMeshInHardward(mesh);
        }
    };

    DistpatchFunctionInHierarchy(node, func);
}

void SceneMaterialTest::Init()
{
    this->tileIndex = 0;

    this->camera->setDepthField(10, 3000);
    this->camera->setView(Vec3(0, -1840, 557), Vec3(0, 30, 20), Vec3(0, 0, 1));
    this->camera->setQueueCullFunc([](int queue) {
        if (queue == RenderQueues::UI) {
            return false;
        }
        return true;
    });

    TextureParser::getInstance().addTextures("Textures/Scenes/", "png|jpg");
    TextureManager::getInstance().loadTextures();

    FilePtr file = FileSystem::getInstance().getFile("Material/Scene/scene.material");
    MaterialLoader::getInstance().Load(file);

    FbxParser *parser = new FbxParser();
    file = FileSystem::getInstance().getFile("Model/scene.data");
    parser->parse(file);

    this->tiles = parser->getNodes();

    for (auto node : tiles) {
        InitNodeShader(node);
    }

    this->createUI();

    this->changeTile(0);
}

static float s = 1;
static float dir = -1;
void SceneMaterialTest::Update()
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

static LabelPtr labelName;

void SceneMaterialTest::createUI()
{
    std::shared_ptr<Font> font = FontManager::getInstance().GetResource("default");
    if (font == nullptr) {
        CreateDefaultFont();

        font = FontManager::getInstance().GetResource("default");
    }

    labelName = CreateNode2DComponent<Label>(font);
    labelName->setText("Name");
    labelName->getComponent<Transform2D>()->setAnchorPoint(Vec2(0.5f, 0.5f));

    auto buttonNext = CreateNode2DComponent<ImageButton>("b_you.png", "b_you.png", "b_you.png");
    auto buttonPrev = CreateNode2DComponent<ImageButton>("b_zuo.png", "b_zuo.png", "b_zuo.png");

    auto buttonClickFunc = [=](ButtonPtr& widget) {
        if (widget == buttonNext) {
            if (this->tileIndex < this->tiles.size() - 1) {
                this->changeTile(this->tileIndex + 1);
            }
        } else if (widget == buttonPrev) {
            if (this->tileIndex > 0) {
                this->changeTile(this->tileIndex - 1);
            }
        }
    };

    buttonNext->setOnClickFunc(buttonClickFunc);
    buttonPrev->setOnClickFunc(buttonClickFunc);

    auto scene = stage->getLastLayer();
    auto window = scene->getComponent<WindowManager>()->pushWindow("HelloWindow");
    window->getNode()->addChild(labelName->getNode());
    window->getNode()->addChild(buttonPrev->getNode());
    window->getNode()->addChild(buttonNext->getNode());

    LayoutUtil::LayoutToParent(labelName->getComponent<Transform2D>(), AlignType::CENTER_BOTTOM, AlignType::CENTER_BOTTOM, 0, 50);
    LayoutUtil::LayoutToParent(buttonPrev->getComponent<Transform2D>(), AlignType::CENTER_BOTTOM, AlignType::CENTER_BOTTOM, -30, 50);
    LayoutUtil::LayoutToParent(buttonNext->getComponent<Transform2D>(), AlignType::CENTER_BOTTOM, AlignType::CENTER_BOTTOM, 30, 50);
}

void SceneMaterialTest::changeTile(int index)
{
    this->tileIndex = index;

    rootNode->removeAllChildren();

    auto blockRootNode = this->tiles[tileIndex];

    rootNode->addChild(blockRootNode);

    labelName->setText(blockRootNode->getName());
}
