#include "TweenTest.h"

#include "UI/Base/Sprite.h"
#include "UI/Base/NinePatch.h"
#include "UI/Base/Label.h"
#include "UI/Base/Node2d.h"

#include "Tween.h"
#include "Timeline.h"
#include "TweenManager.h"
#include "Util/PredefineTweenAccessor.h"

using namespace TweenEngine;

TweenEngine::TweenManager* tweenManager = new TweenEngine::TweenManager();

TweenTest::TweenTest()
{
    this->name = "TweenTest";
}

extern std::shared_ptr<TextureAtlas> CreateDefaultFont();

void TweenTest::Init()
{
    TextureParser::getInstance().addTextures("UI/", "png");
    TextureManager::getInstance().loadTextures();

    SpritePtr sprite = CreateView<Sprite>("store_icon_coin.png");
    sprite->rebind();
    sprite->getTransform2D()->setPosition(Vec2(300, 300 + 200));

    NinePatchPtr patch = CreateView<NinePatch>("tab_press.png");
    patch->setStrethPadding(20, 20, 20, 20);
    patch->setContentSize(Size(200, 100));
    patch->rebind();
    patch->getTransform2D()->setPosition(Vec2(300, 300 + 80));
    patch->setAnchorPoint(Vec2(0.5, 0.5));

    std::shared_ptr<Font> font = FontManager::getInstance().getFont("default");
    if (font == nullptr) {
        CreateDefaultFont();

        font = FontManager::getInstance().getFont("default");
    }

    LabelPtr label = CreateView<Label>(font);
    label->setText("xH<size=50>e<color=FF0000FF>l</color>l</size>o <color=00FFFF>xxo</color>tbo");
    label->getTransform2D()->setPosition(Vec2(300, 500 + 40));

    LabelPtr label2 = CreateView<Label>(font);
    label2->setText("xHtbo xx");
    label2->getTransform2D()->setPosition(Vec2(300, 300 + 40));

    auto scene = stage->getLastLayer();
    auto window = scene->pushWindow("HelloWindow");
    window->addChild(sprite);
    window->addChild(patch);
    window->addChild(label);
    window->addChild(label2);

    PredefineTweenAccessor::registerAccessor();

    Tween::to(label, FlatNodeAccessor::POSITION_X, 1).target(180).delay(1.0f).repeat(5, 0.5).start(*tweenManager);

    Timeline::createSequence()
        // First, set all objects to their initial positions
        .push(Tween::set(sprite, FlatNodeAccessor::ROTATION).target(10))
        .push(Tween::set(label2, FlatNodeAccessor::ROTATION).target(20))
        .push(Tween::set(patch, FlatNodeAccessor::ROTATION).target(30))

        // Wait 1s
        .pushPause(1.0f)

        // Move the objects around, one after the other
        .push(Tween::to(sprite, FlatNodeAccessor::ROTATION, 1.0).target(90))
        .push(Tween::to(label2, FlatNodeAccessor::ROTATION, 2.0).target(90))
        .push(Tween::to(patch, FlatNodeAccessor::ROTATION, 1.0).target(90))

        // Then, move the objects around at the same time
        .beginParallel()
            .push(Tween::to(sprite, FlatNodeAccessor::ROTATION, 1.0).target(180))
            .push(Tween::to(label2, FlatNodeAccessor::ROTATION, 1.0).target(160))
            .push(Tween::to(patch, FlatNodeAccessor::ROTATION, 1.0).target(150))
        .end()

        // And repeat the whole sequence 2 times
        // with a 0.5s pause between each iteration
        .repeatYoyo(5, 0.5f)

        // Let's go!
        .start(*tweenManager);
}

void TweenTest::Update(float dt)
{
    tweenManager->update(1/50.0);
}
