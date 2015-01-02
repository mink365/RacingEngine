#ifndef RE_NATIVEWINDOW_H
#define RE_NATIVEWINDOW_H

#include <android_native_app_glue.h>

#include "Math/Rect.h"
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

    void setVsync(bool enable) override;
    bool isVsync() const override;

    void initView(android_app* app);
    int32_t handleInput(AInputEvent *event);

    int swapBuffers();

public:
    int initDisplay();
    void destroySurface();
    void destroyDisplay();

    int getOrientationAngle();

private:
    void dispatchTouchEvent(TouchEventType type, float x, float y);

private:
    android_app* __app;

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;

    Rect viewRect;

    int orientationAngle;
    bool vsync;
};

} // namespace re

#endif // RE_NATIVEWINDOW_H
