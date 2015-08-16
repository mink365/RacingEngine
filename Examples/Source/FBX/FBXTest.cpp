#include "FBXTest.h"

#include "ModelLoader/fbx/FbxParser.h"
#include "Animation/SkeletonController.h"
#include "Animation/Animation.h"
#include "Animation/AnimationTrack.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"

#include "ShapeGenerater.h"

FBXTest::FBXTest()
{
    this->name = "FBXTest";
}

const static int BLOCK_LENGTH = 157;
const static int BLOCK_COUNT = 5;
std::vector<NodePtr> blocks;
NodePtr black_box;
NodePtr motoRoot;
NodePtr man;

void FBXTest::Init()
{
    TextureParser::getInstance().addTextures("Model/PAD/", "png|jpg");

    this->camera->setDepthField(10, 1320);
    this->camera->setView(Vec3(0, -340, 57), Vec3(0, 30, 20), Vec3(0, 0, 1));
    this->camera->setQueueCullFunc([](int queue) {
        if (queue == RenderQueues::UI) {
            return false;
        }
        return true;
    });

    FbxParser *parser = new FbxParser();

    std::string assertDir = "Model/PAD/";

    FilePtr file = FileSystem::getInstance().getFile((assertDir + "black.data"));
    parser->parse(file);
    NodePtr black = parser->getNodes()[0];

    file = FileSystem::getInstance().getFile((assertDir + "wall.data"));
    parser->parse(file);

    NodePtr wall = parser->getNodes()[0];
    NodePtr floor = parser->getNodes()[1];

    assertDir = "Model/Moto/";

    TextureParser::getInstance().addTextures("Model/Moto/", "png|jpg");
    TextureParser::getInstance().addTextures("Model/Man/", "png|jpg");

    TextureManager::getInstance().loadTextures();

    file = FileSystem::getInstance().getFile((assertDir + "new_group_moto03.data"));
    parser->parse(file);
    NodePtr shadow = parser->getNodes()[0];
    NodePtr moto = parser->getNodes()[1];

    std::vector<NodePtr> meshs;
    meshs.push_back(wall);
    meshs.push_back(floor);
    meshs.push_back(black);
    meshs.push_back(shadow);
    meshs.push_back(moto);

    for (NodePtr node : meshs) {
        auto element = node->getComponent<RenderElement>();

        InitMeshInHardward(element);
    }

    for (int i = 0; i < BLOCK_COUNT; ++i) {
        NodePtr block = CreateNode();

        auto wall_copy = wall->clone();

        auto floor_copy = floor->clone();

        block->addChild(wall_copy);
        block->addChild(floor_copy);

        block->getTransform()->setLocalTranslation(Vec3(0, 0 + BLOCK_LENGTH * (i - 3), 0));

        rootNode->addChild(block);

        blocks.push_back(block);
    }

    motoRoot = CreateNode();
    motoRoot->addChild(shadow);
    motoRoot->addChild(moto);
    motoRoot->getTransform()->setLocalTranslation(Vec3(0, 0, 12));

    rootNode->addChild(motoRoot);
    black_box = black;

    assertDir = "Model/Man/";
    parser->parse(assertDir + "group_girl.data");
    man = parser->getSkinningNode("girl");

    auto animation = man->getComponent<Animation>();
    AnimationTrackPtr track = animation->getCurrAnimationTrack();
    Long beginTime = track->getKeyFrame(2)->getTime();
    Long endTime = track->getKeyFrame(12)->getTime();
    animation->addAnimationStack(std::make_shared<AnimationStack>(beginTime, endTime));
    animation->setAnimationStackIndex(0);
    animation->setAnimationLoop(true);
    animation->setAnimationPower(1.0);
    animation->setIsUseAnimationStack(true);

//    manController->setDefaultFrame(1);
    man->getComponent<SkeletonController>()->play();

    InitMeshInHardward(man->getComponent<RenderElement>());
    rootNode->addChild(man);
}

static float rotation = 0;
void FBXTest::Update()
{
    rotation += 0.04;
    if (rotation > 360) {
        rotation = 0;
    }

//    widget->setRotation(rotation * 20);

    Vec3 position(0, 0, 57);
//    position.x = rotation;
    position.x = MathLib::sin(rotation * DEG_TO_RAD) * 300;
    position.y = MathLib::cos(rotation * DEG_TO_RAD) * 300;

    camera->setView(position, Vec3(0, 30, 20), Vec3(0, 0, 1));

    Quat quat;
    quat.fromAngles(Vec3(0, 0, rotation));
    motoRoot->getTransform()->setLocalRotation(quat);

    MeshPtr mesh = man->getComponent<RenderElement>()->getMesh();

    BufferObjectUtil::getInstance().updateGeometryToHardware(*(mesh.get()));

    man->getTransform()->setLocalRotation(quat);
}
