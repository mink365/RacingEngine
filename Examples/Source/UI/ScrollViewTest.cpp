#include "ScrollViewTest.h"

#include "UI/Widget/ListView/ScrollView.h"

#include "MaskTest.h"

ScrollViewTest::ScrollViewTest()
{
    this->name = "ScrollViewTest";
}

void ScrollViewTest::Init()
{
    Uv uv{1.3f, 30.0f};
    Mat4 mat = Mat4::Identity;
    LogError("xx: {}, {}", uv, mat);

    auto scene = stage->getLastLayer();
    auto window = scene->getComponent<WindowManager>()->pushWindow("HelloWindow");

    auto scrollView = CreateNode2D<ScrollView>();
    scrollView->getComponent<Transform2D>()->setSize(Size(500, 300));
    scrollView->getComponent<Transform2D>()->setAnchor(Vec2(0.5f, 0.5f));
    scrollView->getComponent<Transform2D>()->setPosition(Vec2(700, 600));

    auto patch = MaskTest::CreateNinePatch(800, 300);
    patch->getComponent<Transform2D>()->setAnchor(Vec2(0, 0));

    scrollView->getNode()->addChild(patch->getNode());
    scrollView->setContainer(patch->getComponent<Transform2D>());

    window->getNode()->addChild(scrollView->getNode());
}

void ScrollViewTest::Update()
{

}

