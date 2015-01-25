#include "Widget.h"

#include "UI/Base/HierarchyColor.h"
#include "UI/Base/Transform2D.h"
#include "Layout/LayoutUtil.h"

namespace re {

WidgetPtr GetWidgetComponent(Node& node)
{
    if (node.getComponentCount() < 3) {
        return nullptr;
    }

    // TODO: can't use getComponent<Widget>, Widget is just a base class
    auto component = node.getComponents().at(2);

    // TODO: use UNIFY event/message distpach system
    return std::dynamic_pointer_cast<Widget>(component);
}

Widget::Widget()
    : _touchEnable(true)
    , _blockTouch(true)
{

}

Widget::~Widget() {

}

void Widget::init()
{
    color = this->getComponent<HierarchyColor>();
    transform = this->getComponent<Transform2D>();
}

void Widget::addWidgets() {

}

void Widget::layout() {
    auto func = [](NodePtr& node) {
        auto widget = GetWidgetComponent(*node);

        if (widget) {
            widget->layoutSelf();
        }
    };

    auto node = getNode();
    DistpatchFunctionInHierarchy(node, func);
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
        auto ptr = GetWidgetComponent(*this->getNode());

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

        WidgetPtr childWidget = GetWidgetComponent(*child);

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
    Rect rect(0, 0, transform->getContentSize().width, transform->getContentSize().height);
    
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

ComponentPtr Widget::createCloneInstance() const
{
    return CreateCloneInstance<Widget>();
}

void Widget::copyProperties(const Component *component)
{
    Component::copyProperties(component);

    const Widget* inst = dynamic_cast<const Widget*>(component);
    if (inst) {
        this->_touchEnable = inst->_touchEnable;
        this->_blockTouch = inst->_blockTouch;

        this->state = inst->state;
        this->touchState = WidgetTouchState::TOUCH_CANCLE;

        // TODO: listener

        this->init();
    }
}

}
