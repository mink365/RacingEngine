#include "Widget.h"

#include "UI/Base/HierarchyColor.h"
#include "UI/Base/Transform2D.h"
#include "Layout/LayoutUtil.h"

namespace re {
namespace ui {

Widget::Widget()
    : _touchEnable(true)
    , _blockTouch(true)
{

}

Widget::~Widget() {

}

void Widget::onAwake()
{
    color = this->getComponent<HierarchyColor>();
    transform = this->getComponent<Transform2D>();
}

void Widget::addWidgets() {

}

void Widget::layout() {
    auto func = [](NodePtr& node) {
        auto widget = node->getComponent<Widget>();

        if (widget) {
            widget->layoutSelf();
        }
    };

    auto node = getNode();
    DistpatchFunctionInHierarchy(node, func);
}

void Widget::registerEvents()
{
    Component::registerEvents();

    RegisterEvent(Events::Awake, this, &Widget::onAwake);
}

void Widget::layoutSelf() {

}

void Widget::setTouchEnabled(bool value) {
    this->_touchEnable = value;
}

bool Widget::isTouchEnabled() {
    return this->_touchEnable;
}

bool Widget::isBlockTouch() {
    return this->_blockTouch;
}

void Widget::setBlockTouch(bool value) {
    _blockTouch = value;
}

void Widget::addTouchListener(TouchEventListener::ptr listener)
{
    this->_onTouchListeners.push_back(listener);
}

bool Widget::onTouchEvent(TouchEvent &event)
{
    if (!this->_touchEnable || !this->isVisible()) {
        return false;
    }

    auto curr = event.getCurrPoint();

    // handle event for self
    WidgetTouchState oldTouchState = this->touchState;
    WidgetTouchState newTouchState = this->touchState;
    switch (event.getType()) {
    case TouchEventType::DOWN:
        if (this->hit(curr)) {
            newTouchState = WidgetTouchState::TOUCH_DOWN_INSIDE;
        } else {
            newTouchState = WidgetTouchState::TOUCH_DOWN_OUTSIDE;
        }

        break;
    case TouchEventType::MOVE:
        if (this->hit(curr)) {
            newTouchState = WidgetTouchState::MOVE_INSIDE;
        } else {
            newTouchState = WidgetTouchState::MOVE_OUTSIDE;
        }
        break;

    case TouchEventType::UP:
        if (this->hit(curr)) {
            newTouchState = WidgetTouchState::TOUCHUP_INSIDE;
        } else {
            newTouchState = WidgetTouchState::TOUCHUP_OUTSIDE;
        }
        break;
    case TouchEventType::CANCEL:
        newTouchState = WidgetTouchState::TOUCH_CANCLE;
    }

    bool handled;

    handled = this->emitWidgetTouchEvent(oldTouchState, newTouchState, event);

    this->touchState = newTouchState;

    //////////// transform to children /////////////////////////
    bool childHandled = this->dispatchTouchEvent(event);
    if (childHandled) {
        handled = true;
    }

    return handled;
}

bool Widget::emitWidgetTouchEvent(WidgetTouchState oldTouchState, WidgetTouchState newTouchState, TouchEvent &event)
{
    if (_onTouchListeners.size() == 0) {
        return false;
    }

    // 只有down事件是阻塞的，即，某个widget发生了down事件，就返回了，事件不再扩散
    bool r = false;
    bool lr = false;
    for (auto listener : this->_onTouchListeners) {
        auto ptr = this->getComponent<Widget>();

        if (oldTouchState != newTouchState) {
            listener->onTouchStateChange(oldTouchState, newTouchState, event, ptr);
        }

        switch (newTouchState) {
        case WidgetTouchState::TOUCH_DOWN_INSIDE:
            lr = listener->onTouchDown(event, ptr);
            break;
        case WidgetTouchState::MOVE_INSIDE:
            listener->onTouchMoveInside(event, ptr);
            if (oldTouchState == WidgetTouchState::MOVE_OUTSIDE) {
                listener->onTouchMoveEnter(event, ptr);
            }
            break;
        case WidgetTouchState::MOVE_OUTSIDE:
            listener->onTouchMoveOutside(event, ptr);
            if (oldTouchState == WidgetTouchState::MOVE_INSIDE) {
                listener->onTouchMoveOut(event, ptr);
            }
            break;
        case WidgetTouchState::TOUCHUP_INSIDE:
            listener->onTouchUpInside(event, ptr);
            break;
        case WidgetTouchState::TOUCHUP_OUTSIDE:
            listener->onTouchUpOutside(event, ptr);
            break;
        case WidgetTouchState::TOUCH_CANCLE:
            listener->onTouchCancle(event, ptr);
            break;
        case WidgetTouchState::TOUCH_DOWN_OUTSIDE:
            listener->onTouchDownOutSide(event, ptr);
            break;
        default:
            break;
        }

        if (lr == true) {
            r = true;
        }
    }

    return r;
}

bool Widget::dispatchTouchEvent(TouchEvent &event)
{
    auto curr = event.getCurrPoint();

    bool handled = false;

    // 靠前的控件先接受事件
    int size = getNode()->getChildren().size();
    for (int i = size - 1; i >= 0; --i) {
        auto child = getNode()->getChildren()[i];

        WidgetPtr childWidget = child->getComponent<Widget>();

        if (!childWidget) {
            continue;
        }

        Vec2 p = childWidget->getComponent<Transform2D>()->convertParentToLocalSpace(curr);
        event.setCurrPoint(p);

        // 有控件处理了事件就阻止传递
        if (childWidget->onTouchEvent(event)) {
            handled = true;

            break;
        }
    }

    return handled;
}

bool Widget::hitFromWorldPoint(const Vec2 &p) {
    Vec2 point = transform->convertToNodeSpace(p);
    
    return this->hit(point);
}

bool Widget::hit(const Vec2 &p) {
    // rect local space
    Rect rect(0, 0, transform->getSize().width, transform->getSize().height);
    
    if (rect.containsPoint(p)) {
        return true;
    }
    
    return false;
}

bool Widget::hitTest(Transform2DPtr node, Vec2 p) {
    if(!isRunning() || !isVisible()){
        return false;
    }
    
    Vec2 localP = transform->convertToNodeSpace(p);
    
    Rect rect = node->getBoundingBox();
    
    if (rect.containsPoint(localP)) {
        return true;
    }
    
    return false;
}

void Widget::copyProperties(const Widget &rhs)
{
    this->_touchEnable = rhs._touchEnable;
    this->_blockTouch = rhs._blockTouch;

    this->touchState = WidgetTouchState::TOUCH_CANCLE;
}

} // namespace ui
} // namespace re
