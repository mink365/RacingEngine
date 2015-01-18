#include "UITest.h"

#include "UI/Base/Sprite.h"
#include "UI/Base/NinePatch.h"
#include "UI/Base/Label.h"
#include "UI/Manager/UIManager.h"
#include "UI/Widget/Button.h"
#include "Font/FontManager.h"
#include "Font/TextureAtlas.h"
#include "Font/FreeTypeUtil.h"

UITest::UITest()
{
    this->name = "UITest";
}

extern std::shared_ptr<TextureAtlas> CreateDefaultFont();

void UITest::Init()
{
    SpritePtr sprite = CreateNode2DComponent<Sprite>("store_icon_coin.png");
    sprite->rebind();
    sprite->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 200));

    NinePatchPtr patch = CreateNode2DComponent<NinePatch>("tab_press.png");
    patch->setStrethPadding(20, 20, 20, 20);
    patch->getComponent<Transform2D>()->setContentSize(Size(200, 100));
    patch->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 80));
    patch->getComponent<Transform2D>()->setAnchorPoint(Vec2(0.5, 0.5));
    patch->rebind();

    FontPtr font = FontManager::getInstance().getFont("default");
    if (font == nullptr) {
        CreateDefaultFont();

        font = FontManager::getInstance().getFont("default");
    }

    LabelPtr label = CreateNode2DComponent<Label>(font);
    label->setText("xH<size=50>e<color=FF0000FF>l</color>l</size>o <color=00FFFF>xxo</color>tbo");

    LabelPtr label2 = CreateNode2DComponent<Label>(font);
    label2->setText("xHtbo xx");

    auto button = CreateNode2DComponent<ImageButton>("rate.png", "rate_press.png", "rate.png");

    auto scene = stage->getLastLayer();
    auto window = scene->pushWindow("HelloWindow");
    window->getNode()->addChild(sprite->getNode());
    window->getNode()->addChild(patch->getNode());
    window->getNode()->addChild(label->getNode());
    window->getNode()->addChild(label2->getNode());
    window->getNode()->addChild(button->getNode());

//    window->setPosition(Vec2(300, 300));

    LayoutUtil::LayoutToParentCenter(button->getComponent<Transform2D>());

    Size windowSize = window->getComponent<Transform2D>()->getContentSize();

    label->getComponent<Transform2D>()->setContentSize(Size(200, 50));
    label->getComponent<Transform2D>()->setAnchorPoint(Vec2(0.5, 0.5));
    label->getComponent<Transform2D>()->setPosition(Vec2(windowSize.width/2.0, windowSize.height / 2.0));

    label2->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 40));

    auto buttonClickFunc = [=](ButtonPtr& widget) {
        auto win = this->createWin();

        scene->pushWindow(win);
    };

    button->setOnClickFunc(buttonClickFunc);
}

void UITest::Update(float dt)
{

}

std::shared_ptr<Window> UITest::createWin()
{
    auto win = CreateNode2DComponent<Window>();
    win->getComponent<Transform2D>()->setContentSize(Size(400, 500));

    NinePatchPtr patch = CreateNode2DComponent<NinePatch>("tab_press.png");
    patch->setStrethPadding(20, 20, 20, 20);
    patch->getComponent<Transform2D>()->setContentSize(Size(400, 500));
    patch->getComponent<Transform2D>()->setAnchorPoint(Vec2(0.5, 0.5));
    patch->rebind();

    auto button = CreateNode2DComponent<ImageButton>("btn_close_normal.png", "btn_close_press.png", "btn_close_normal.png");

    win->getNode()->addChild(patch->getNode());
    win->getNode()->addChild(button->getNode());

    LayoutUtil::LayoutToParentCenter(patch->getComponent<Transform2D>());
    LayoutUtil::LayoutToParentRightTop(button->getComponent<Transform2D>());

    auto buttonClickFunc = [=](ButtonPtr& widget) {
        win->popFromWindowManager();
    };

    button->setOnClickFunc(buttonClickFunc);

    return win;
}
