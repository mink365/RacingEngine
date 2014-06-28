#ifndef WIDGETCONTAINER_H
#define WIDGETCONTAINER_H

#include "Base/Node2d.h"
#include "Layout/Screen.h"

#include <string>
using namespace std;

namespace re {

namespace cocos2d {
    namespace ui {
        class Widget;
    }
}

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
//    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
//    virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
//    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
//    virtual void ccTouchCancelled(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    
    bool isTouchEnabled();
    void setTouchEnabled(bool value);
    
    bool isBlockTouch();
    void setBlockTouch(bool value);
    
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
    
public:
    bool hitFromWorldPoint(const Vec2& p);
    bool hit(const Vec2& p);
    
    bool hitTest(Node2d::ptr node, Vec2 p);
//    CCMenuItem* findMenuForTouch(CCTouch* pTouch);
    
//protected:
//    bool checkEventWidget(cocos2d::ui::Widget *root, CCTouch* touch, CCEvent *pEvent);
//    bool checkTouchEvent(cocos2d::ui::Widget* root, CCTouch* touch, CCEvent* pEvent);
    
//public:
//    void registerMenuItem(cocos2d::CCMenuItem* item);
    
private:
//    cocos2d::CCMenuItem* _selectedItem;
//    cocos2d::CCMenuItem* _touchBeginItem;
    
//    CCArray* m_pSelectedWidgets;
    
    bool _touchEnable;
    /*
     * 是否阻挡touch，如果阻挡，所有进入此空间范围内的点击会被吞掉
     * 默认是吞掉的
     */
    bool _blockTouch;
};

}

#endif // WIDGETCONTAINER_H
