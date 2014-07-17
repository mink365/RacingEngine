#include "FontTest.h"

#include "Font/FontManager.h"
#include "Font/Font.h"
#include "Font/TextureAtlas.h"
#include "Font/FreeTypeUtil.h"
#include "ShapeGenerater.h"

FontTest::FontTest()
{
}

std::shared_ptr<TextureAtlas> CreateDefaultFont()
{
    std::string resDir = "/home/jk/workspace/engines/RacingEngine/Examples/Resources/";

    auto fontFile = resDir + "Fonts/ObelixPro.ttf";

    auto font = std::make_shared<Font>(128, fontFile.c_str());
    font->setName("default");
    FontManager::getInstance().registerFont(font);

    auto atlas = TextureAtlas::create();
    atlas->init(512, 512, 1);

    FreeTypeUtil::LoadGlyphs(atlas, font, L"Hello People我xtb");

    atlas->upload();

    return atlas;
}

void FontTest::Init()
{
    auto atlas = CreateDefaultFont();

    MeshPtr mesh = ShapeGenerater::getInstance().CreateBox(50, atlas->getTexture());
    InitMeshInHardward(mesh);

    box = std::make_shared<SceneNode>();
    AddMeshToNode(box, mesh);

    box->setLocalTranslation(Vec3(0, 0, 52));

    rootNode->addChild(box);
}

static float rotation = 0;
void FontTest::Update(float dt)
{
    rotation += 0.04;
    if (rotation > 360) {
        rotation = 0;
    }

    Quat quat;
    quat.fromAngles(Vec3(0, 0, rotation));

    box->setLocalRotation(quat);
}
