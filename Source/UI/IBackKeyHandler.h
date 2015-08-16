#ifndef __IBackKeyHandler__
#define __IBackKeyHandler__

#include <iostream>

namespace re {
namespace ui {

class IBackKeyHandler {
public:
    virtual bool onBackKeyEvent() = 0;
};

} // namespace ui
} // namespace re

#endif /* defined(__IBackKeyHandler__) */
