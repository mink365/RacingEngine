//
//  IBackKeyHandler.h
//  MT
//
//  Created by 寇 瑞 on 9/9/13.
//
//

#ifndef __MT__IBackKeyHandler__
#define __MT__IBackKeyHandler__

#include <iostream>

namespace re {

class IBackKeyHandler {
public:
    virtual bool onBackKeyEvent() = 0;
};

}

#endif /* defined(__MT__IBackKeyHandler__) */
