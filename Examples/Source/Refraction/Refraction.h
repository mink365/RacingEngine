#ifndef REFRACTION_H
#define REFRACTION_H

#include "BaseTest.h"

class Refraction : public BaseTest
{
public:
    Refraction();

    virtual void Init();
    virtual void Update();

private:
    void setupShader();
};

#endif // REFRACTION_H
