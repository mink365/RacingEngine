#ifndef WIDGETCONTAINER_H
#define WIDGETCONTAINER_H

#include "Component.h"
#include "UI/Base/HierarchyColor.h"
#include "UI/Base/Transform2D.h"
#include "Layout/Screen.h"
#include "TouchEvent.h"

#include <string>
using namespace std;

namespace re {

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

    void init();
    
    void layout();

    void update(float delta);
    
    bool isVisible() {return true;};
    bool isRunning() {return true;};

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
    virtual void addWidgets();
    
    void dispatchLayout();
    virtual void layoutSelf();
    
    void dispatchUpdate(float delta);
    virtual void updateSelf(float delta);
    
    /*
     * 插入剪切等效果
     */
    virtual void beforeDraw();
    virtual void afterDraw();

    virtual void initView() {};

protected:
    virtual ComponentPtr createCloneInstance() const;
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

}

#endif // WIDGETCONTAINER_H
