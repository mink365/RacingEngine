#include "ScrollView.h"
#include "UI/Layout/LayoutUtil.h"
#include "Math/Rect.h"
#include "Util/LocalTime.h"

namespace re {
namespace ui {

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

    listener->onTouchDown = [&](TouchEvent& event, WidgetPtr&) {
        this->handleTouchDownEvent(event);

        return true;
    };

    listener->onTouchMoveInside = [&](TouchEvent& event, WidgetPtr&) {
        this->handleTouchMoveEvent(event);
    };

    listener->onTouchMoveOutside = [&](TouchEvent& event, WidgetPtr&) {
        this->handleTouchMoveEvent(event);
    };

    listener->onTouchUpInside = [&](TouchEvent& event, WidgetPtr&) {
        this->handleTouchUpEvent(event);
    };

    listener->onTouchUpOutside = [&](TouchEvent& event, WidgetPtr&) {
        this->handleTouchMoveEvent(event);
    };

    this->_onTouchListeners.push_back(listener);
}

void ScrollView::update()
{
    switch (param.scrollType) {
    case FlickableDirection::All:
    case FlickableDirection::Auto:
        hAxis.update();
        vAxis.update();
        break;
    case FlickableDirection::Horizonal:
        hAxis.update();
    case FlickableDirection::Vertical:
        vAxis.update();
    default:
        break;
    }

    container->setPosition(Vec2(hAxis.currentPos, vAxis.currentPos));
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

    tracker.addPoint(currTouch, LocalTime::getInstance().getCurrentTime());

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

    tracker.computeVelocity();
    const Vec2& velocity = tracker.getVelocity();

    if (this->isHFlickable()) {
        if (MathLib::abs(velocity.x) >= param.miniFlickVeclocity
                && MathLib::abs(diffTouch.x) >= param.flickThreshhold) {
            hAxis.flick(velocity.x);
        } else {
            hAxis.fixup();
        }
    }

    if (this->isVFlickable()) {
        if (MathLib::abs(velocity.y) >= param.miniFlickVeclocity
                && MathLib::abs(diffTouch.y) >= param.flickThreshhold) {
            vAxis.flick(velocity.y);
        } else {
            vAxis.fixup();
        }
    }
}

} // namespace ui
} // namespace re

