#include "Widget.h"

#include "Layout/LayoutUtil.h"

namespace re {

Widget::Widget()
    : _touchEnable(true)
    , _blockTouch(true)
{
//    this->setCascadeOpacityEnabled(true);
//    this->setCascadeColorEnabled(true);
}

Widget::~Widget() {
//    m_pSelectedWidgets->removeAllObjects();
//    m_pSelectedWidgets->release();
}

bool Widget::init() {
    if (!Node2d::init()) {
        return false;
    }
    
//    m_pSelectedWidgets = CCArray::create();
//    m_pSelectedWidgets->retain();
    
//    this->setCascadeOpacityEnabled(true);
//    this->setCascadeColorEnabled(true);
    
    return true;
}

void Widget::addWidgets() {

}

void Widget::layout() {
//    CCAssert(this->getParent(), "you should add it to scene before you can layout the layer");

    this->dispatchLayout();
    
    this->layoutSelf();
    
    this->dispatchLayout();
}

void Widget::dispatchLayout() {
    for (auto child : this->getChildren()) {
        std::shared_ptr<Widget> childWidget = dynamic_pointer_cast<Widget>(child);

        if (childWidget) {
            childWidget->layout();
        }
    }
}

void Widget::layoutSelf() {

}

void Widget::update(float delta) {
    this->dispatchUpdate(delta);
    
    this->updateSelf(delta);
}

void Widget::dispatchUpdate(float delta) {
    for (auto child : this->getChildren()) {
        std::shared_ptr<Widget> childWidget = dynamic_pointer_cast<Widget>(child);

        if (childWidget) {
            childWidget->update(delta);
        }
    }
}

void Widget::updateSelf(float delta) {

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

bool Widget::hitFromWorldPoint(const Vec2 &p) {
    Vec2 point = convertToNodeSpace(p);
    
    return this->hit(point);
}

bool Widget::hit(const Vec2 &p) {
    // rect local space
    Rect rect(0, 0, getContentSize().width, getContentSize().height);
    
    if (rect.containsPoint(p)) {
        return true;
    }
    
    return false;
}

bool Widget::hitTest(Node2d::ptr node, Vec2 p) {
    if(!isRunning() || !isVisible()){
        return false;
    }
    
    Vec2 localP = convertToNodeSpace(p);
    
    Rect rect = node->getBoundingBox();
    
    if (rect.containsPoint(localP)) {
        return true;
    }
    
    return false;
}

//bool Widget::checkEventWidget(cocos2d::ui::Widget *root, CCTouch* touch, CCEvent *pEvent)
//{
//    checkTouchEvent(root,touch, pEvent);
//    return (m_pSelectedWidgets->count() > 0);
//}

//bool Widget::checkTouchEvent(cocos2d::ui::Widget *root, CCTouch* touch, CCEvent* pEvent)
//{
//    ccArray* arrayRootChildren = root->getChildren()->data;
//    int length = arrayRootChildren->num;
//    for (int i=length-1; i >= 0; i--)
//    {
//        cocos2d::ui::Widget* widget = (cocos2d::ui::Widget*)(arrayRootChildren->arr[i]);
//        if (checkTouchEvent(widget, touch, pEvent))
//        {
//            return true;
//        }
//    }
//    bool pass = root->onTouchBegan(touch, pEvent);
//    if (root->isHitted())
//    {
//        m_pSelectedWidgets->addObject(root);
//        return true;
//    }
//    return pass;
//}

//bool Widget::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
//    if(!isRunning() || !isVisible() || !isTouchEnabled()){
//        return false;
//    }
    
//    // uselect all item
//    _selectedItem = NULL;
    
//    bool handled = (_blockTouch && hitFromWorldPoint(pTouch->getLocation()));
    
//    if (this->getChildren() == NULL) {
//        return handled;
//    }
    
//    int size = this->getChildren()->count();
//    for (int i = size - 1; i >= 0; --i) {
//        CCObject* child = this->getChildren()->objectAtIndex(i);
//        CCTouchDelegate* touch = dynamic_cast<CCTouchDelegate*>(child);
//        cocos2d::ui::Widget* uiWidget = dynamic_cast<cocos2d::ui::Widget*>(child);
        
//        if (touch != NULL) {
//            if (touch->ccTouchBegan(pTouch, pEvent)) {
//                handled = true;
                
//                return true;
//            }
//        } else if (uiWidget != nullptr) {
//            // widget下得所有的控件的点击遍历等，与我们的widgetContainer完全不一样的
//            return checkEventWidget(uiWidget, pTouch, pEvent);
//        }
        
//        CCMenuItem* item = dynamic_cast<CCMenuItem*>(child);
//        if (!_selectedItem && item != NULL
//            && item->isEnabled()
//            && item->isVisible()
//            && hitTest(item, pTouch->getLocation())) {
//            _selectedItem = item;
//            _selectedItem->selected();
            
//            _touchBeginItem = item;
            
//            handled = true;
            
//            return true;
//        }
//    }
    
//    return handled;
//}

//void Widget::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
//    if(!isRunning() || !isVisible() || !isTouchEnabled()){
//        return;
//    }
    
//    if (this->getChildren() != NULL) {
//        int size = this->getChildren()->count();
//        for (int i = size - 1; i >= 0; --i) {
//            CCTouchDelegate* child = dynamic_cast<CCTouchDelegate*>(this->getChildren()->objectAtIndex(i));
            
//            if (child != NULL) {
//                child->ccTouchMoved(pTouch, pEvent);
//            }
//        }
//    }
    
//    // ccs
//    ccArray* selectedWidgetArray = m_pSelectedWidgets->data;
//    int length = selectedWidgetArray->num;
//    for (int i=0; i<length; ++i)
//    {
//        cocos2d::ui::Widget* hitWidget = (cocos2d::ui::Widget*)(selectedWidgetArray->arr[i]);
//        hitWidget->onTouchMoved(pTouch, pEvent);
//    }
    
//    if (_selectedItem) {
//        if (!hitTest(_selectedItem, pTouch->getLocation())) {
//            // we have to find other item
//            _selectedItem->unselected();
            
//            _selectedItem = this->findMenuForTouch(pTouch);
//            if (_selectedItem != NULL) {
//                _selectedItem->selected();
//            }
//        }
//    }
//}

//void Widget::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
//    if(!isRunning() || !isVisible() || !isTouchEnabled()){
//        return;
//    }
    
//    if (this->getChildren() != NULL) {
//        int size = this->getChildren()->count();
//        for (int i = size - 1; i >= 0; --i) {
//            CCTouchDelegate* child = dynamic_cast<CCTouchDelegate*>(this->getChildren()->objectAtIndex(i));
            
//            if (child != NULL) {
//                child->ccTouchEnded(pTouch, pEvent);
//            }
//        }
//    }
    
//    // ccs
//    ccArray* selectedWidgetArray = m_pSelectedWidgets->data;
//    int length = selectedWidgetArray->num;
//    for (int i=0; i<length; ++i)
//    {
//        cocos2d::ui::Widget* hitWidget = (cocos2d::ui::Widget*)(selectedWidgetArray->arr[0]);
//        m_pSelectedWidgets->removeObject(hitWidget);
//        hitWidget->onTouchEnded(pTouch, pEvent);
//    }
    
//    if (_selectedItem) {
//        _selectedItem->unselected();
        
//        if (_touchBeginItem == _selectedItem) {
//            _selectedItem->activate();
//        }
//        _selectedItem = NULL;
//    }
    
//    _touchBeginItem = NULL;
//}

//void Widget::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
//    if(!isRunning() || !isVisible() || !isTouchEnabled()){
//        return;
//    }
    
//    if (this->getChildren() != NULL) {
//        int size = this->getChildren()->count();
//        for (int i = size - 1; i >= 0; --i) {
//            CCTouchDelegate* child = dynamic_cast<CCTouchDelegate*>(this->getChildren()->objectAtIndex(i));
            
//            if (child != NULL) {
//                child->ccTouchCancelled(pTouch, pEvent);
//            }
//        }
//    }
    
//    // ccs
//    ccArray* selectedWidgetArray = m_pSelectedWidgets->data;
//    int length = selectedWidgetArray->num;
//    for (int i=0; i<length; ++i)
//    {
//        cocos2d::ui::Widget* hitWidget = (cocos2d::ui::Widget*)(selectedWidgetArray->arr[0]);
//        m_pSelectedWidgets->removeObject(hitWidget);
//        hitWidget->onTouchCancelled(pTouch, pEvent);
//    }
    
//    if (_selectedItem) {
//        _selectedItem->unselected();
//        _selectedItem = NULL;
//    }
    
//    _touchBeginItem = NULL;
//}

//CCMenuItem* Widget::findMenuForTouch(CCTouch* touch) {
//    if (this->getChildren() == NULL) {
//        return NULL;
//    }
    
//    int size = this->getChildren()->count();
//    for (int i = size - 1; i >= 0; --i) {
//        CCMenuItem* item = dynamic_cast<CCMenuItem*>(this->getChildren()->objectAtIndex(i));
        
//        if (item != NULL && item->isVisible() && item->isEnabled()) {
//            if(hitTest(item, touch->getLocation())) {
//                return item;
//            }
//        }
//    }
    
//    return NULL;
//}

//void Widget::registerMenuItem(cocos2d::CCMenuItem* item) {

//}

void Widget::visit() {
	// quick return if not visible
	if (!isVisible())
    {
		return;
    }
    
//	kmGLPushMatrix();
	
//    if (m_pGrid && m_pGrid->isActive())
//    {
//        m_pGrid->beforeDraw();
//        this->transformAncestors();
//    }
    
//	this->transform();
    
//    this->beforeDraw();
    
//	if(m_pChildren)
//    {
//        sortAllChildren();
        
//		ccArray *arrayData = m_pChildren->data;
//		unsigned int i=0;
		
//		// draw children zOrder < 0
//		for( ; i < arrayData->num; i++ )
//        {
//			CCNode *child =  (CCNode*)arrayData->arr[i];
//			if ( child->getZOrder() < 0 )
//            {
//				child->visit();
//			}
//            else
//            {
//				break;
//            }
//		}
		
//		// this draw
//		this->draw();
		
//		// draw children zOrder >= 0
//		for( ; i < arrayData->num; i++ )
//        {
//			CCNode* child = (CCNode*)arrayData->arr[i];
//			child->visit();
//		}
        
//	} else {
//		this->draw();
//    }
    
//    // reset for next frame
//    m_uOrderOfArrival = 0;
    
//    this->afterDraw();
    
//	if ( m_pGrid && m_pGrid->isActive())
//    {
//		m_pGrid->afterDraw(this);
//    }
    
//	kmGLPopMatrix();
}

void Widget::beforeDraw() {
}

void Widget::afterDraw() {
}

}
