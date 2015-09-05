#include "UITest.h"

#include "UI/Base/Image.h"
#include "UI/Base/Text.h"
#include "UI/Manager/UIManager.h"
#include "UI/Widget/Button.h"
#include "Font/TextureAtlas.h"
#include "Font/FreeTypeUtil.h"

UITest::UITest()
{
    this->name = "UITest";
}

extern TextureAtlasPtr CreateDefaultFont();

void UITest::Init()
{
    ImagePtr sprite = CreateNode2D<Image>();
    sprite->setFrame("store_icon_coin.png");
    sprite->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 200));

    auto patch = CreateNode2D<Image>();
    patch->setType(ImageType::NinePatch);
    patch->getData<ImageType::NinePatch>() = {20, 20, 20, 20};
    patch->setFrame("tab_press.png");
    patch->getComponent<Transform2D>()->setSize(Size(200, 100));
    patch->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 80));
    patch->getComponent<Transform2D>()->setAnchor(Vec2(0.5, 0.5));

    TextPtr label = CreateNode2D<Text>();
    label->setText("xH<size=50>e<color=FF0000FF>l</color>l</size>o <color=00FFFF>xxo</color>tbo");

    TextPtr label2 = CreateNode2D<Text>();
    label2->setText("xHtbo xx");

    auto button = CreateImageButton("rate.png", "rate_press.png", "rate.png");

    auto scene = stage->getLastLayer();
    auto window = scene->getComponent<WindowManager>()->pushWindow("HelloWindow");

    window->getNode()->addChild(sprite->getNode());
    window->getNode()->addChild(patch->getNode());
    window->getNode()->addChild(label->getNode());
    window->getNode()->addChild(label2->getNode());
    window->getNode()->addChild(button->getNode());

//    window->setPosition(Vec2(300, 300));

    LayoutUtil::LayoutToParent(button->getComponent<Transform2D>(), AlignType::CENTER, AlignType::CENTER);

    Size windowSize = window->getComponent<Transform2D>()->getSize();

    label->getComponent<Transform2D>()->setSize(Size(200, 50));
    label->getComponent<Transform2D>()->setAnchor(Vec2(0.5, 0.5));
    label->getComponent<Transform2D>()->setPosition(Vec2(windowSize.width/2.0, windowSize.height / 2.0));

    label2->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 40));

    auto buttonClickFunc = [=](ButtonPtr& widget) {
        auto win = this->createWin();

        scene->getComponent<WindowManager>()->pushWindow(win);
    };

    button->setOnClickFunc(buttonClickFunc);
}

void UITest::Update()
{

}

WindowPtr UITest::createWin()
{
    auto win = CreateNode2D<Window>();
    win->getNode()->setName("Window1");
    win->getComponent<Transform2D>()->setSize(Size(400, 500));

    auto patch = CreateNode2D<Image>();
    patch->setType(ImageType::NinePatch);
    patch->getData<ImageType::NinePatch>() = {20, 20, 20, 20};
    patch->setFrame("tab_press.png");
    patch->getComponent<Transform2D>()->setSize(Size(400, 500));
    patch->getComponent<Transform2D>()->setAnchor(Vec2(0.5, 0.5));

    auto button = CreateImageButton("btn_close_normal.png", "btn_close_press.png", "btn_close_normal.png");

    win->getNode()->addChild(patch->getNode());
    win->getNode()->addChild(button->getNode());

    LayoutUtil::LayoutToParent(patch->getComponent<Transform2D>(), AlignType::CENTER, AlignType::CENTER);
    LayoutUtil::LayoutToParent(button->getComponent<Transform2D>(), AlignType::RIGHT_TOP, AlignType::RIGHT_TOP);

    auto buttonClickFunc = [=](ButtonPtr& widget) {
        win.getPtr()->popFromWindowManager();
    };

    button->setOnClickFunc(buttonClickFunc);

    return win;
}
