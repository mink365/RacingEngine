#ifndef RE_NATIVEWINDOW_H
#define RE_NATIVEWINDOW_H

#include "Math/Rect.h"
#include "Platform/NativeView.h"

namespace re {

class NativeWindow : public NativeView
{
public:
    NativeWindow();
    
    void setFrameSize(float width, float height) override;
    Size getFrameSize() const override;

private:
    Size framebufferSize;
};

} // namespace re

#endif // RE_NATIVEWINDOW_H