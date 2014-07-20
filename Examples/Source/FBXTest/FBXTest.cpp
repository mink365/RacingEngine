#include "FBXTest.h"

#include "ModelLoader/fbx/FbxParser.h"
#include "Animation/SkeletonController.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/BoneNode.h"

#include "ShapeGenerater.h"

FBXTest::FBXTest()
{
}

const static int BLOCK_LENGTH = 157;
const static int BLOCK_COUNT = 5;
std::vector<SceneNodePtr> blocks;
SceneNodePtr black_box;
SceneNodePtr motoRoot;
SkeletonControllerPtr manController;

void FBXTest::Init()
{
    TextureParser::getInstance().addTextures("Model/PAD/", "png|jpg");

    FbxParser *parser = new FbxParser();

    std::string assertDir = "Model/PAD/";

    FilePtr file = FileSystem::getInstance().openFile((assertDir + "black.data"));
    parser->parse(file);
    SceneNodePtr black = parser->getNodes()[0];

    file = FileSystem::getInstance().openFile((assertDir + "wall.data"));
    parser->parse(file);

    SceneNodePtr wall = parser->getNodes()[0];
    SceneNodePtr floor = parser->getNodes()[1];

    assertDir = "Model/Moto/";

    TextureParser::getInstance().addTextures("Model/Moto/", "png|jpg");
    TextureParser::getInstance().addTextures("Model/Man/", "png|jpg");

    TextureManager::getInstance().loadTextures();

    file = FileSystem::getInstance().openFile((assertDir + "new_group_moto03.data"));
    parser->parse(file);
    SceneNodePtr shadow = parser->getNodes()[0];
    SceneNodePtr moto = parser->getNodes()[1];

    std::vector<SceneNodePtr> meshs;
    meshs.push_back(wall);
    meshs.push_back(floor);
    meshs.push_back(black);
    meshs.push_back(shadow);
    meshs.push_back(moto);

    for (SceneNodePtr node : meshs) {
        MeshPtr mesh = dynamic_pointer_cast<Mesh>(node->getNodeAttribute());

        InitMeshInHardward(mesh);
    }

    for (int i = 0; i < BLOCK_COUNT; ++i) {
        SceneNodePtr block = std::make_shared<SceneNode>();

        auto wall_copy = wall->clone();

        auto floor_copy = floor->clone();

        block->addChild(wall_copy);
        block->addChild(floor_copy);

        block->setLocalTranslation(Vec3(0, 0 + BLOCK_LENGTH * (i - 3), 0));

        rootNode->addChild(block);

        blocks.push_back(block);
    }

    motoRoot = std::make_shared<SceneNode>();
    motoRoot->addChild(shadow);
    motoRoot->addChild(moto);
    motoRoot->setLocalTranslation(Vec3(0, 0, 12));

    // TODO:
//    moto->getMaterial()->getRenderState().setDepthTest(true);

    rootNode->addChild(motoRoot);
    black_box = black;

    assertDir = "Model/Man/";
    parser->parse(assertDir + "group_girl.data");
    manController = parser->getSkeletonController("girl");

    AnimationTrackPtr track = manController->getAnimation()->getCurrAnimationTrack();
    Long beginTime = track->getKeyFrame(2)->getTime();
    Long endTime = track->getKeyFrame(12)->getTime();
    manController->getAnimation()->addAnimationStack(std::make_shared<AnimationStack>(beginTime, endTime));
    manController->getAnimation()->setAnimationStackIndex(0);
    manController->getAnimation()->setAnimationLoop(true);
    manController->getAnimation()->setAnimationPower(1.0);
    manController->getAnimation()->setIsUseAnimationStack(true);

//    manController->setDefaultFrame(1);

    InitMeshInHardward(manController->getMesh());
    rootNode->addChild(manController->getMeshNode());

//    MeshPtr mesh = ShapeGenerater::getInstance().CreateBox(2);
//    InitMeshInHardward(mesh);

//    auto box = std::make_shared<SceneNode>();
//    AddMeshToNode(box, mesh);
}

static float rotation = 0;
void FBXTest::Update(float dt)
{
    rotation += 0.04;
    if (rotation > 360) {
        rotation = 0;
    }

//    widget->setRotation(rotation * 20);

    Quat quat;
    quat.fromAngles(Vec3(0, 0, rotation));
    motoRoot->setLocalRotation(quat);

    manController->update();
    MeshPtr mesh = manController->getMesh();

    Geometry::ptr geometry = mesh->getGeometry();
    BufferObjectUtil::getInstance().updateGeometryToHardware(*(geometry.get()));

    mesh->getNode()->setLocalRotation(quat);
}
