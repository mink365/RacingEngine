#include "UITest.h"

#include "UI/Base/Sprite.h"
#include "UI/Base/NinePatch.h"
#include "UI/Base/Label.h"
#include "UI/Base/Node2d.h"
#include "UI/Manager/UISceneManager.h"
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
    TextureParser::getInstance().addTextures("UI/", "png");
    TextureManager::getInstance().loadTextures();

    SpritePtr sprite = std::make_shared<Sprite>("store_icon_coin.png");
    sprite->rebind();
    sprite->setPosition(Vec2(300, 300 + 200));

    NinePatchPtr patch = std::make_shared<NinePatch>("tab_press.png");
    patch->setStrethPadding(20, 20, 20, 20);
    patch->setContentSize(Size(200, 100));
    patch->rebind();
    patch->setPosition(Vec2(300, 300 + 80));
    patch->setAnchorPoint(Vec2(0.5, 0.5));

    std::shared_ptr<Font> font = FontManager::getInstance().getFont("default");
    if (font == nullptr) {
        CreateDefaultFont();

        font = FontManager::getInstance().getFont("default");
    }

    LabelPtr label = std::make_shared<Label>();
    label->init(font);
    label->setText("xH<size=50>e<color=FF0000FF>l</color>l</size>o <color=00FFFF>xxo</color>tbo");

    LabelPtr label2 = std::make_shared<Label>();
    label2->init(font);
    label2->setText("xHtbo xx");

    label2->setPosition(Vec2(300, 300 + 40));

    auto button = CreateView<ImageButton>();
    button->initView("rate.png", "rate_press.png", "rate.png");

    auto scene = stage->getLastLayer();
    auto window = scene->pushWindow("HelloWindow");
    window->addChild(sprite);
    window->addChild(patch);
    window->addChild(label);
    window->addChild(label2);
    window->addChild(button);

//    window->setPosition(Vec2(300, 300));

    LayoutUtil::layoutParentCenter(button);

    label->setContentSize(Size(200, 50));
    label->setAnchorPoint(Vec2(0.5, 0.5));
    label->setPosition(window->getContentSize().width/2.0, window->getContentSize().height / 2.0);
}

void UITest::Update(float dt)
{

}
