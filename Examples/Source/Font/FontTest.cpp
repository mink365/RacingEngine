#include "FontTest.h"

#include "Font/Font.h"
#include "Font/TextureAtlas.h"
#include "Font/FreeTypeUtil.h"
#include "ShapeGenerater.h"
#include "FileSystem/FileSystem.h"

FontTest::FontTest()
{
    this->name = "FontTest";
}

TextureAtlasPtr CreateDefaultFont()
{
    auto fontFile = "Fonts/Arial Rounded MT Bold.ttf";
    FilePtr file = FileSystem::getInstance().getFile(fontFile);

    auto font = Create<Font>(FontType::TTF, 32, file);
    font->setName("default");
    FontManager::instance().Register(font);

    auto atlas = TextureAtlas::create();
    atlas->init(1024, 1024, 1);

    FreeTypeUtil::LoadGlyphs(atlas, font, L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");

    atlas->upload();

    return atlas;
}

void FontTest::Init()
{
    auto atlas = CreateDefaultFont();

    auto box = ShapeGenerater::instance().CreateBox(150, 150, 150);

    boxNode = CreateMeshEntity();
    SetMeshData(boxNode, box, atlas->getTexture());

    boxNode->getTransform()->setLocalTranslation(Vec3(0, 0, 52));

    rootNode->addChild(boxNode->getNode());
}

static float rotation = 0;
void FontTest::Update()
{
    rotation += 0.04 * 0.3;
    if (rotation > 360) {
        rotation = 0;
    }

    Quat quat;
    quat.fromAngles(Vec3(0, 0, rotation));

    boxNode->getTransform()->setLocalRotation(quat);
}
