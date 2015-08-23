#ifndef MASKTEST_H
#define MASKTEST_H

#include "BaseTest.h"

class MaskTest : public BaseTest
{
public:
    MaskTest();

    virtual void Init();
    virtual void Update();

private:
    NinePatchPtr CreateNinePatch(int width, int height);
};

#endif // MASKTEST_H
