#ifndef RE_NATIVEWINDOW_H
#define RE_NATIVEWINDOW_H

#include "Math/BoundingVolume.h"

class GLFWwindow;

namespace re {

class NativeWindow
{
public:
    NativeWindow();

    bool initView();
    void bindEventHandler();

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
