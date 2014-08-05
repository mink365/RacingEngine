#ifndef RE_NATIVEWINDOW_H
#define RE_NATIVEWINDOW_H

#include <android_native_app_glue.h>

#include "Math/BoundingVolume.h"
#include "Platform/NativeView.h"
#include "UI/TouchEvent.h"

#include <EGL/egl.h>
#include <GLES/gl.h>

namespace re {

class engine;

class NativeWindow : public NativeView
{
public:
    NativeWindow();

    void setFrameSize(float width, float height) override;
    Size getFrameSize() const override;

    void initView(android_app* app);
    int32_t handleInput(AInputEvent *event);

    int swapBuffers();
public:
    int initDisplay();
    void termDisplay();

private:
    void dispatchTouchEvent(TouchEventType type, float x, float y);

private:
    android_app* __app;

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    Rect viewRect;
};

} // namespace re

#endif // RE_NATIVEWINDOW_H
