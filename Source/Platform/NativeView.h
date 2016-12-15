#ifndef NATIVEVIEW_H
#define NATIVEVIEW_H

#include "Math/Rect.h"

namespace re {

class NativeView
{
public:
    virtual void setFrameSize(float width, float height) = 0;
    virtual Size getFrameSize() const = 0;

    virtual void setVsync(bool) {}
    virtual bool isVsync() const
    {
        return false;
    }
};

}

#endif // NATIVEVIEW_H
