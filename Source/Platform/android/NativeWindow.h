#ifndef RE_NATIVEWINDOW_H
#define RE_NATIVEWINDOW_H

#include <android_native_app_glue.h>

#include "Math/BoundingVolume.h"
#include "Platform/NativeView.h"

namespace re {

class engine;

class NativeWindow : public NativeView
{
public:
    NativeWindow();

    void setFrameSize(float width, float height) override;
    Size getFrameSize() const override;

    void initView();
protected:
    int initDisplay(engine* engine);
    void termDisplay(engine* engine);

    static int32_t handleInput(android_app* app, AInputEvent* event);
    static void handleCmd(android_app* app, int32_t cmd);
};

} // namespace re

#endif // RE_NATIVEWINDOW_H
