#include "TweenTest.h"

#include "UI/Base/Sprite.h"
#include "UI/Base/NinePatch.h"
#include "UI/Base/Label.h"

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

extern TextureAtlasPtr CreateDefaultFont();

void TweenTest::Init()
{
    TextureParser::instance().addTextures("UI/", "png");
    TextureManager::instance().loadTextures();

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
    label->getComponent<Transform2D>()->setPosition(Vec2(300, 500 + 40));

    LabelPtr label2 = CreateUIGraphicNode<Label>(font);
    label2->setText("xHtbo xx");
    label2->getComponent<Transform2D>()->setPosition(Vec2(300, 300 + 40));

    auto scene = stage->getLastLayer();
    auto window = scene->getComponent<WindowManager>()->pushWindow("HelloWindow");
    window->getNode()->addChild(sprite->getNode());
    window->getNode()->addChild(patch->getNode());
    window->getNode()->addChild(label->getNode());
    window->getNode()->addChild(label2->getNode());

    Tween::to(label->getNode(), FlatNodeAccessor::POSITION_X, 1).target(180).delay(1.0f).repeat(5, 0.5).start(*tweenManager);

    Timeline::createSequence()
        // First, set all objects to their initial positions
        .push(Tween::set(sprite->getNode(), FlatNodeAccessor::ROTATION).target(10))
        .push(Tween::set(label2->getNode(), FlatNodeAccessor::ROTATION).target(20))
        .push(Tween::set(patch->getNode(), FlatNodeAccessor::ROTATION).target(30))

        // Wait 1s
        .pushPause(1.0f)

        // Move the objects around, one after the other
        .push(Tween::to(sprite->getNode(), FlatNodeAccessor::ROTATION, 1.0).target(90))
        .push(Tween::to(label2->getNode(), FlatNodeAccessor::ROTATION, 2.0).target(90))
        .push(Tween::to(patch->getNode(), FlatNodeAccessor::ROTATION, 1.0).target(90))

        // Then, move the objects around at the same time
        .beginParallel()
            .push(Tween::to(sprite->getNode(), FlatNodeAccessor::ROTATION, 1.0).target(180))
            .push(Tween::to(label2->getNode(), FlatNodeAccessor::ROTATION, 1.0).target(160))
            .push(Tween::to(patch->getNode(), FlatNodeAccessor::ROTATION, 1.0).target(150))
        .end()

        // And repeat the whole sequence 2 times
        // with a 0.5s pause between each iteration
        .repeatYoyo(5, 0.5f)

        // Let's go!
        .start(*tweenManager);
}

void TweenTest::Update()
{
    tweenManager->update(1/50.0);
}
