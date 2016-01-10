#ifndef RE_NATIVEWINDOW_H
#define RE_NATIVEWINDOW_H

#include "Math/Rect.h"
#include "Platform/NativeView.h"

#include "UI/TouchEvent.h"

namespace re {

class NativeWindow : public NativeView
{
public:
    NativeWindow();
    
    void setFrameSize(float width, float height) override;
    Size getFrameSize() const override;

public:
    void dispatchTouchEvent(ui::TouchEventType type, float x, float y);

private:
    Rect viewRect;
};

} // namespace re


#ifdef __cplusplus
extern "C" {
#endif

    void view_touch_begin(float x, float y);
    void view_touch_moved(float x, float y);
    void view_touch_ended(float x, float y);
    void view_touch_cancled(float x, float y);

#ifdef __cplusplus
}
#endif

#endif // RE_NATIVEWINDOW_H