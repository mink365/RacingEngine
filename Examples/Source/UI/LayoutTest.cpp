#include "LayoutTest.h"
#include "UI/Base/Sprite.h"
#include "UI/Layout/HorizontalLayoutGroup.h"
#include "UI/Layout/VerticalLayoutGroup.h"

LayoutTest::LayoutTest()
{
    this->name = "LayoutTest";
}

LayoutTest::~LayoutTest()
{

}

void LayoutTest::Init()
{
    auto scene = stage->getLastLayer();
    auto window = scene->getComponent<WindowManager>()->pushWindow("HelloWindow");

    {
        auto container = CreateNode2DComponent<Widget>();
        container->getComponent<Transform2D>()->setSize(Size(500, 100));

        auto alignment = Alignment::create();
        alignment->alignFrom = LayoutUtil::AlignTypeToPoint(AlignType::CENTER);
        alignment->alignTo = LayoutUtil::AlignTypeToPoint(AlignType::CENTER);
        alignment->offset = Vec2(0, 0 + 0);
        container->getComponent<LayoutElement>()->setAlignment(alignment);

        auto group = std::make_shared<HorizontalLayoutGroup>();
        group->SetAlign(LinearAlign::Left);
        container->getEntity()->addComponent(group);
        container->getEntity()->setName("Container");

        for (size_t i = 0; i < 5; ++i) {
            SpritePtr sprite = CreateNode2DComponent<Sprite>("store_icon_coin.png");
            sprite->rebind();
            sprite->getNode()->setName("Sprite");

            auto alignment = Alignment::create();
            alignment->alignFrom = LayoutUtil::AlignTypeToPoint(AlignType::CENTER);
            alignment->alignTo = LayoutUtil::AlignTypeToPoint(AlignType::CENTER);
            alignment->offset = Vec2(0, 0 + i * 40);
            sprite->getComponent<LayoutElement>()->setAlignment(alignment);

            container->getNode()->addChild(sprite->getNode());
        }

        window->getNode()->addChild(container->getNode());
    }

    {
        auto container = CreateNode2DComponent<Widget>();
        container->getComponent<Transform2D>()->setSize(Size(100, 200));

        auto alignment = Alignment::create();
        alignment->alignFrom = LayoutUtil::AlignTypeToPoint(AlignType::CENTER);
        alignment->alignTo = LayoutUtil::AlignTypeToPoint(AlignType::CENTER);
        alignment->offset = Vec2(0, 0 + 0);
        container->getComponent<LayoutElement>()->setAlignment(alignment);

        auto group = std::make_shared<VerticalLayoutGroup>();
        group->SetSpacing(-20);
        group->SetAlign(LinearAlign::Center);
        container->getEntity()->addComponent(group);
        container->getEntity()->setName("Container");

        for (size_t i = 0; i < 5; ++i) {
            SpritePtr sprite = CreateNode2DComponent<Sprite>("store_icon_coin.png");
            sprite->rebind();
            sprite->getNode()->setName("Sprite");

            auto alignment = Alignment::create();
            alignment->alignFrom = LayoutUtil::AlignTypeToPoint(AlignType::CENTER);
            alignment->alignTo = LayoutUtil::AlignTypeToPoint(AlignType::CENTER);
            alignment->offset = Vec2(0, 0 + i * 40);
            sprite->getComponent<LayoutElement>()->setAlignment(alignment);

            container->getNode()->addChild(sprite->getNode());
        }

        window->getNode()->addChild(container->getNode());
    }

    root = window->getNode();
    root->setName("Window");
}

void LayoutTest::Update()
{
    LayoutRoot(root);
}

