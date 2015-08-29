#ifndef WIDGETCONTAINER_H
#define WIDGETCONTAINER_H

#include "Scene/Node.h"
#include "UI/Base/HierarchyColor.h"
#include "UI/Base/Transform2D.h"
#include "UI/Layout/LayoutElement.h"
#include "UI/Layout/LayoutGroup.h"
#include "Layout/Screen.h"
#include "TouchEvent.h"

#include <string>

namespace re {
namespace ui {

enum class WidgetState {
    DEFAULT,
    PRESSED,
    SELECTED,
    DISABLED,
};

class Widget : public Component, public Shared<Widget>
{
public:
    Widget();
    virtual ~Widget();

    void layout();

    bool isVisible() {return true;};
    bool isRunning() {return true;};

public:
    void registerEvents();

public:
    bool isTouchEnabled();
    void setTouchEnabled(bool value);
    
    bool isBlockTouch();
    void setBlockTouch(bool value);

public:
    virtual bool onTouchEvent(TouchEvent& event);
    
protected:
    bool emitWidgetTouchEvent(WidgetTouchState oldState, WidgetTouchState newState, TouchEvent& event);
    bool dispatchTouchEvent(TouchEvent& event);

protected:
    void onAwake();

protected:
    virtual void addWidgets();
    
    virtual void layoutSelf();

    virtual void initView() {};

protected:
    virtual ComponentPtr createCloneInstance() const override;
    virtual void copyProperties(const Component* component) override;
    
public:
    bool hitFromWorldPoint(const Vec2& p);
    bool hit(const Vec2& p);
    
    bool hitTest(Transform2DPtr node, Vec2 p);
    
protected:
    bool _touchEnable;
    /*
     * 是否阻挡touch，如果阻挡，所有进入此空间范围内的点击会被吞掉
     * 默认是吞掉的
     */
    bool _blockTouch;

    WidgetState state;
    WidgetTouchState touchState;

    std::vector<TouchEventListener::ptr> _onTouchListeners;

protected:
    HierarchyColorPtr color;
    Transform2DPtr transform;
};

} // namespace ui
} // namespace re

#endif // WIDGETCONTAINER_H
