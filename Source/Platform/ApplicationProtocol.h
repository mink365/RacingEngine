#ifndef APPLICATIONPROTOCOL_H
#define APPLICATIONPROTOCOL_H

#include "NativeView.h"

namespace re {

class ApplicationProtocol {
public:
    virtual void initViewSize() {};

    virtual bool initEnvironment() = 0;

    virtual void onEnterForeground() = 0;
    virtual void onExitForeground() = 0;

    NativeView* getNativeView() const;

protected:
    NativeView* view;
};

}

#endif // APPLICATIONPROTOCOL_H
