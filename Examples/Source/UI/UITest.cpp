#include "UITest.h"

#include "UI/Base/Sprite.h"
#include "UI/Base/NinePatch.h"
#include "UI/Base/Label.h"
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
    SpritePtr sprite = CreateUIGraphicNode<Sprite>("store_icon_coin.png");
    sprite->rebind();
    sprite->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 200));

    NinePatchPtr patch = CreateUIGraphicNode<NinePatch>("tab_press.png");
    patch->setStrethPadding(20, 20, 20, 20);
    patch->getComponent<Transform2D>()->setSize(Size(200, 100));
    patch->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 80));
    patch->getComponent<Transform2D>()->setAnchor(Vec2(0.5, 0.5));
    patch->rebind();

    FontPtr font = FontManager::instance().GetResource("default");
    if (font == nullptr) {
        CreateDefaultFont();

        font = FontManager::instance().GetResource("default");
    }

    LabelPtr label = CreateUIGraphicNode<Label>(font);
    label->setText("xH<size=50>e<color=FF0000FF>l</color>l</size>o <color=00FFFF>xxo</color>tbo");

    LabelPtr label2 = CreateUIGraphicNode<Label>(font);
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

    NinePatchPtr patch = CreateUIGraphicNode<NinePatch>("tab_press.png");
    patch->setStrethPadding(20, 20, 20, 20);
    patch->getComponent<Transform2D>()->setSize(Size(400, 500));
    patch->getComponent<Transform2D>()->setAnchor(Vec2(0.5, 0.5));
    patch->rebind();

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
