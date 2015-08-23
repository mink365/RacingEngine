#include "ScrollViewTest.h"

#include "UI/Base/NinePatch.h"
#include "UI/Widget/ListView/ScrollView.h"

#include "MaskTest.h"

ScrollViewTest::ScrollViewTest()
{
    this->name = "ScrollViewTest";
}

void ScrollViewTest::Init()
{
    auto scene = stage->getLastLayer();
    auto window = scene->getComponent<WindowManager>()->pushWindow("HelloWindow");

    auto scrollView = CreateNode2D<ScrollView>();
    scrollView->getComponent<Transform2D>()->setSize(Size(200, 200));
    scrollView->getComponent<Transform2D>()->setPosition(Vec2(600, 600));

    NinePatchPtr patch = MaskTest::CreateNinePatch(300, 300);

    scrollView->getNode()->addChild(patch->getNode());
    scrollView->setContainer(patch->getComponent<Transform2D>());

    window->getNode()->addChild(scrollView->getNode());
}

void ScrollViewTest::Update()
{

}

