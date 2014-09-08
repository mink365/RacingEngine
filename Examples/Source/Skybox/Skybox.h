#ifndef SKYBOX_H
#define SKYBOX_H

#include "BaseTest.h"

class Skybox : public BaseTest
{
public:
    Skybox();

    virtual void Init();
    virtual void Update(float dt);
};

#endif // SKYBOX_H
