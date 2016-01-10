#include "NativeWindow.h"

#include "Math/Rect.h"
#include <string>
#include "UI/TouchEvent.h"
#include "Message/MessageManager.h"
#include "Message/MessageConstant.h"

#include <stdlib.h>

static re::NativeWindow* _window;

namespace re {
    
using namespace ui;

NativeWindow::NativeWindow()
{
    _window = this;
}

void NativeWindow::setFrameSize(float width, float height)
{
    this->viewRect.size = Size(width, height);
}

Size NativeWindow::getFrameSize() const
{
    return this->viewRect.size;
}

void NativeWindow::dispatchTouchEvent(TouchEventType type, float x, float y)
{
    auto event = std::make_shared<TouchEvent>();

    const Rect& rect = this->viewRect;
        
    event->setInfo(type, x - rect.origin.x,
                       rect.size.height - (y - rect.origin.y));
        
    MessageManager::instance().sendNoKeyMessage(MessageConstant::MessageType::TOUCHSCREEN_MESSAGE, event);
}
    
}

#ifdef __cplusplus
extern "C" {
#endif
    
    void view_touch_begin(float x, float y)
    {
        _window->dispatchTouchEvent(re::ui::TouchEventType::DOWN, x, y);
    }
    
    void view_touch_moved(float x, float y)
    {
        _window->dispatchTouchEvent(re::ui::TouchEventType::MOVE, x, y);
    }
    
    void view_touch_ended(float x, float y)
    {
        _window->dispatchTouchEvent(re::ui::TouchEventType::UP, x, y);
    }
    
    void view_touch_cancled(float x, float y)
    {
        _window->dispatchTouchEvent(re::ui::TouchEventType::CANCEL, x, y);
    }
    
#ifdef __cplusplus
}
#endif
