#ifndef LAYOUTTEST_H
#define LAYOUTTEST_H

#include "BaseTest.h"

class LayoutTest : public BaseTest
{
public:
    LayoutTest();
    ~LayoutTest();

    virtual void Init();
    virtual void Update();

private:
    NodePtr root;
};

#endif // LAYOUTTEST_H
