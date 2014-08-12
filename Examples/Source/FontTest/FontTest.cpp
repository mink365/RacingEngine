#include "FontTest.h"

#include "Font/FontManager.h"
#include "Font/Font.h"
#include "Font/TextureAtlas.h"
#include "Font/FreeTypeUtil.h"
#include "ShapeGenerater.h"
#include "FileSystem/FileSystem.h"

FontTest::FontTest()
{
    this->name = "FontTest";
}

std::shared_ptr<TextureAtlas> CreateDefaultFont()
{
    auto fontFile = "Fonts/ObelixPro.ttf";
    FilePtr file = FileSystem::getInstance().openFile(fontFile);

    auto font = std::make_shared<Font>(32, file);
    font->setName("default");
    FontManager::getInstance().registerFont(font);

    auto atlas = TextureAtlas::create();
    atlas->init(1024, 1024, 1);

    FreeTypeUtil::LoadGlyphs(atlas, font, L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~");

    atlas->upload();

    return atlas;
}

void FontTest::Init()
{
    auto atlas = CreateDefaultFont();

    MeshPtr mesh = ShapeGenerater::getInstance().CreateBox(150, atlas->getTexture());
    InitMeshInHardward(mesh);

    box = std::make_shared<SceneNode>();
    AddMeshToNode(box, mesh);

    box->setLocalTranslation(Vec3(0, 0, 52));

    rootNode->addChild(box);
}

static float rotation = 0;
void FontTest::Update(float dt)
{
    rotation += 0.04 * 0.3;
    if (rotation > 360) {
        rotation = 0;
    }

    Quat quat;
    quat.fromAngles(Vec3(0, 0, rotation));

    box->setLocalRotation(quat);
}
