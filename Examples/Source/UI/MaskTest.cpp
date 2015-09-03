#include "MaskTest.h"
#include "UI/Base/Mask.h"
#include "UI/Base/Maskable.h"

MaskTest::MaskTest()
{
    this->name = "MaskTest";
}

void MaskTest::Init()
{
    auto scene = stage->getLastLayer();
    auto window = scene->getComponent<WindowManager>()->pushWindow("HelloWindow");

    auto mask = CreateNinePatch(200, 100);
    mask->getEntity()->addComponent<Mask>();
    mask->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 80));

    auto maskable = CreateNinePatch(200, 200);
    maskable->getEntity()->addComponent<Maskable>();
    maskable->getComponent<Transform2D>()->setPosition(Vec2(50, 0));

    mask->getNode()->addChild(maskable->getNode());
    window->getNode()->addChild(mask->getNode());

    mask->getComponent<Mask>()->UpdateInternalState();
    maskable->getComponent<Maskable>()->UpdateInternalState();
}

void MaskTest::Update()
{

}

ImagePtr MaskTest::CreateNinePatch(int width, int height)
{
    auto patch = CreateUIGraphicNode<Image>("tab_press.png");
    patch->getData<ImageType::NinePatch>() = {20, 20, 20, 20};
    patch->getComponent<Transform2D>()->setSize(Size(width, height));
    patch->getComponent<Transform2D>()->setAnchor(Vec2(0.5, 0.5));

    return patch;
}

