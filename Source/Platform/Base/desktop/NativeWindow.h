#ifndef RE_NATIVEWINDOW_H
#define RE_NATIVEWINDOW_H

#include "Math/Rect.h"
#include "Platform/NativeView.h"

struct GLFWwindow;

namespace re {

class NativeWindow : public NativeView
{
    friend class GLFWEventHandler;

public:
    NativeWindow();

    bool initView();
    void bindEventHandler();

    void setFrameSize(float width, float height) override;
    Size getFrameSize() const override;

    bool shouldClose();
    void swapBuffers();
    void pollEvents();

protected:
    bool initGlew();
private:
    GLFWwindow *window;
    Rect viewRect;
};

} // namespace re

#endif // RE_NATIVEWINDOW_H
