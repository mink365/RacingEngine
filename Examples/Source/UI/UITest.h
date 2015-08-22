#ifndef UITEST_H
#define UITEST_H

#include "BaseTest.h"

class UITest : public BaseTest
{
public:
    UITest();

    virtual void Init();
    virtual void Update();

private:
    WindowPtr createWin();
};

#endif // UITEST_H
