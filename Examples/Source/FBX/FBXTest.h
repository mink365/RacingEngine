#ifndef FBXTEST_H
#define FBXTEST_H

#include "BaseTest.h"

class FBXTest : public BaseTest
{
public:
    FBXTest();

    virtual void Init();
    virtual void Update(float dt);
};

#endif // FBXTEST_H
