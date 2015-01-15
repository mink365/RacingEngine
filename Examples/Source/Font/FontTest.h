#ifndef FONTTEST_H
#define FONTTEST_H

#include "BaseTest.h"

class FontTest : public BaseTest
{
public:
    FontTest();

    virtual void Init();
    virtual void Update(float dt);

private:
    NodePtr box;
};

#endif // FONTTEST_H
