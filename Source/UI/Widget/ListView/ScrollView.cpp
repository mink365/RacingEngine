#include "ScrollView.h"
#include "UI/Layout/LayoutUtil.h"
#include "Math/Rect.h"

namespace re
{

bool ScrollView::isHFlickable()
{
    switch(this->param.scrollType)
    {
    case FlickableDirection::Auto:
        if (container->getBoundingBox().getWidth() >= this->getComponent<Transform2D>()->getSize().width) {
            return true;
        }
        break;
    case FlickableDirection::Horizonal:
    case FlickableDirection::All:
        return true;
    case FlickableDirection::Vertical:
        return false;
    }

    return false;
}

bool ScrollView::isVFlickable()
{
    switch(this->param.scrollType)
    {
    case FlickableDirection::Auto:
        if (container->getBoundingBox().getHeight() >= this->getComponent<Transform2D>()->getSize().height) {
            return true;
        }
        break;
    case FlickableDirection::Vertical:
    case FlickableDirection::All:
        return true;
    case FlickableDirection::Horizonal:
        return false;
    }

    return false;
}

Vec2 ScrollView::GetMinExtent()
{
    Vec2 leftBottom = LayoutUtil::CalculateLayoutToParent(container, AlignType::RIGHT_TOP, AlignType::RIGHT_TOP, -hAxis.endMargin, -vAxis.endMargin);

    return leftBottom;
}

Vec2 ScrollView::GetMaxExtent()
{
    Vec2 rightTop = LayoutUtil::CalculateLayoutToParent(container, AlignType::LEFT_BOTTOM, AlignType::LEFT_BOTTOM, hAxis.startMargin, vAxis.startMargin);

    return rightTop;
}

void ScrollView::initTouchListener()
{
    auto listener = TouchEventListener::create();

    listener->onTouchDown = [&](TouchEvent&, WidgetPtr&){

        return true;
    };

    this->_onTouchListeners.push_back(listener);
}

void ScrollView::update()
{

}

void ScrollView::resetAxis()
{
    hAxis.reset();
    vAxis.reset();

    Vec2 min = this->GetMinExtent();
    Vec2 max = this->GetMaxExtent();
    hAxis.minBound = min.x;
    hAxis.maxBound = max.x;
    hAxis.axisLength = container->getBoundingBox().getWidth();
    hAxis.visibleLength = transform->getSize().width;

    vAxis.minBound = min.y;
    vAxis.maxBound = max.y;
    vAxis.axisLength = container->getBoundingBox().getHeight();
    vAxis.visibleLength = transform->getSize().height;

    hAxis.currentPos = transform->getLocalTranslation().x;
    vAxis.currentPos = transform->getLocalTranslation().y;
}

void ScrollView::handleTouchDownEvent(TouchEvent &event)
{
    this->resetAxis();

    hAxis.pressPos = hAxis.currentPos;
    vAxis.pressPos = vAxis.currentPos;

    pressTouch = event.getCurrPoint();
    currTouch = event.getCurrPoint();
    prevTouch = event.getCurrPoint();
}

void ScrollView::handleTouchMoveEvent(TouchEvent &event)
{
    prevTouch = currTouch;
    currTouch = event.getCurrPoint();

    diffTouch = currTouch - pressTouch;

    if (!this->isMoved && diffTouch.lengthSqr() > 144) {
        this->isMoved = true;
    }

    if (!this->isMoved) {
        return;
    }

    if (this->isHFlickable()) {
        hAxis.drag(diffTouch.x);
    }

    if (this->isVFlickable()) {
        vAxis.drag(diffTouch.y);
    }
}

void ScrollView::handleTouchUpEvent(TouchEvent &event)
{
    currTouch = event.getCurrPoint();

    diffTouch = currTouch - pressTouch;


}

}

