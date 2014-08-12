#ifndef __IBackKeyHandler__
#define __IBackKeyHandler__

#include <iostream>

namespace re {

class IBackKeyHandler {
public:
    virtual bool onBackKeyEvent() = 0;
};

}

#endif /* defined(__IBackKeyHandler__) */
