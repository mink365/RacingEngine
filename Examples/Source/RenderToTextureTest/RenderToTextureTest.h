#ifndef RENDERTOTEXTURETEST_H
#define RENDERTOTEXTURETEST_H

#include "BaseTest.h"

class RenderToTextureTest : public BaseTest
{
public:
    RenderToTextureTest();

    virtual void Init();
    virtual void Update(float dt);

protected:
    void RenderScene1();
};

#endif // RENDERTOTEXTURETEST_H
