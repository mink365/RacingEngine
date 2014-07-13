#ifndef WIDGETCONTAINER_H
#define WIDGETCONTAINER_H

#include "Base/Node2d.h"
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

class Widget : public Node2d, public Shared<Widget>
{
public:
    Widget();
    virtual ~Widget();
    
    bool init();
    
    void layout();

    void update(float delta);
    
    void visit();
    
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
    virtual NodePtr createCloneInstance() const;
    virtual void copyProperties(const Node* node) override;
    
public:
    bool hitFromWorldPoint(const Vec2& p);
    bool hit(const Vec2& p);
    
    bool hitTest(Node2d::ptr node, Vec2 p);
    
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
};

}

#endif // WIDGETCONTAINER_H
