#include "UITest.h"

#include "UI/Base/Sprite.h"
#include "UI/Base/NinePatch.h"
#include "UI/Base/Label.h"
#include "UI/Base/Node2d.h"
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
    SpritePtr sprite = CreateView<Sprite>("store_icon_coin.png");
    sprite->rebind();
    sprite->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 200));

    NinePatchPtr patch = CreateView<NinePatch>("tab_press.png");
    patch->setStrethPadding(20, 20, 20, 20);
    patch->setContentSize(Size(200, 100));
    patch->rebind();
    patch->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 80));
    patch->setAnchorPoint(Vec2(0.5, 0.5));

    std::shared_ptr<Font> font = FontManager::getInstance().getFont("default");
    if (font == nullptr) {
        CreateDefaultFont();

        font = FontManager::getInstance().getFont("default");
    }

    LabelPtr label = CreateView<Label>(font);
    label->setText("xH<size=50>e<color=FF0000FF>l</color>l</size>o <color=00FFFF>xxo</color>tbo");

    LabelPtr label2 = CreateView<Label>(font);
    label2->setText("xHtbo xx");

    label2->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 40));

    auto button = CreateView<ImageButton>("rate.png", "rate_press.png", "rate.png");

    auto scene = stage->getLastLayer();
    auto window = scene->pushWindow("HelloWindow");
    window->addChild(sprite);
    window->addChild(patch);
    window->addChild(label);
    window->addChild(label2);
    window->addChild(button);

//    window->setPosition(Vec2(300, 300));

    LayoutUtil::LayoutToParentCenter(button);

    label->setContentSize(Size(200, 50));
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->getComponent<Transform2D>()->setPosition(window->getContentSize().width/2.0, window->getContentSize().height / 2.0);

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
    auto win = CreateView<Window>();
    win->setContentSize(Size(400, 500));

    NinePatchPtr patch = CreateView<NinePatch>("tab_press.png");
    patch->setStrethPadding(20, 20, 20, 20);
    patch->setContentSize(Size(400, 500));
    patch->rebind();
    patch->setAnchorPoint(Vec2(0.5, 0.5));

    auto button = CreateView<ImageButton>("btn_close_normal.png", "btn_close_press.png", "btn_close_normal.png");

    win->addChild(patch);
    win->addChild(button);

    LayoutUtil::LayoutToParentCenter(patch);
    LayoutUtil::LayoutToParentRightTop(button);

    auto buttonClickFunc = [=](ButtonPtr& widget) {
        win->popFromWindowManager();
    };

    button->setOnClickFunc(buttonClickFunc);

    return win;
}
