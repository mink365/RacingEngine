#ifndef LIGHTTEST_H
#define LIGHTTEST_H

#include "BaseTest.h"

class LightTest : public BaseTest
{
public:
    LightTest();

    virtual void Init();
    virtual void Update(float dt);
};

#endif // LIGHTTEST_H