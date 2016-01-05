#include "NativeWindow.h"

#include "Math/Rect.h"
#include <string>
#include "UI/TouchEvent.h"
#include "Message/MessageManager.h"
#include "Message/MessageConstant.h"

#include <stdlib.h>

namespace re {

NativeWindow::NativeWindow()
{
    this->framebufferSize = Size(800, 600);
}

void NativeWindow::setFrameSize(float width, float height)
{
    this->framebufferSize.set(width, height);
}

Size NativeWindow::getFrameSize() const
{
    return this->framebufferSize;
}

}