#include "SceneMaterialTest.h"
#include "Material/Material.h"
#include "Material/MaterialManager.h"
#include "Loader/MaterialLoader.h"
#include "Resource/ModelLoader/fbx/FbxParser.h"
#include "Resource/ResourceManager.h"
#include "UI/Base/Sprite.h"
#include "UI/Base/Label.h"
#include "UI/Base/Node2d.h"
#include "UI/Manager/UIManager.h"
#include "UI/Widget/Button.h"
#include "UI/Layout/LayoutUtil.h"

extern std::shared_ptr<TextureAtlas> CreateDefaultFont();

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
    this->tileIndex = 0;

    this->camera->setDepthField(10, 3000);
    this->camera->setView(Vec3(0, -1840, 557), Vec3(0, 30, 20), Vec3(0, 0, 1));
    this->camera->setQueueCullFunc([](int queue) {
        if (queue == RENDER_QUEUE_UI) {
            return false;
        }
        return true;
    });

    TextureParser::getInstance().addTextures("Textures/Scenes/", "png|jpg");
    TextureManager::getInstance().loadTextures();

    FilePtr file = FileSystem::getInstance().openFile("Material/Scene/scene.material");
    MaterialLoader::getInstance().Load(file);

    FbxParser *parser = new FbxParser();
    file = FileSystem::getInstance().openFile("Model/scene.data");
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

static LabelPtr labelName;

void SceneMaterialTest::createUI()
{
    std::shared_ptr<Font> font = FontManager::getInstance().getFont("default");
    if (font == nullptr) {
        CreateDefaultFont();

        font = FontManager::getInstance().getFont("default");
    }

    labelName = std::make_shared<Label>();
    labelName->init(font);
    labelName->setText("Name");

    auto buttonNext = CreateView<ImageButton>();
    buttonNext->initView("b_you.png", "b_you.png", "b_you.png");
    auto buttonPrev = CreateView<ImageButton>();
    buttonPrev->initView("b_zuo.png", "b_zuo.png", "b_zuo.png");

    auto buttonClickFunc = [=](WidgetPtr& widget) {
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
    auto window = scene->pushWindow("HelloWindow");
    window->addChild(labelName);
    window->addChild(buttonPrev);
    window->addChild(buttonNext);

    LayoutUtil::layoutParent(labelName, AlignType::CENTER_BOTTOM, AlignType::CENTER_BOTTOM, 0, 50);
    LayoutUtil::layoutParent(buttonPrev, AlignType::CENTER_BOTTOM, AlignType::CENTER_BOTTOM, -30, 50);
    LayoutUtil::layoutParent(buttonNext, AlignType::CENTER_BOTTOM, AlignType::CENTER_BOTTOM, 30, 50);
}

void SceneMaterialTest::changeTile(int index)
{
    this->tileIndex = index;

    rootNode->removeAllChildren();

    auto blockRootNode = this->tiles[tileIndex];

    rootNode->addChild(blockRootNode);
}
