#ifndef REFLECTION_H
#define REFLECTION_H

#include "BaseTest.h"

class Reflection : public BaseTest
{
public:
    Reflection();

    virtual void Init();
    virtual void Update();

private:
    void setupShader();
};

#endif // REFLECTION_H
