#ifndef LIGHTMAP_H
#define LIGHTMAP_H

#include "BaseTest.h"

class Lightmap : public BaseTest
{
public:
    Lightmap();

    virtual void Init();
    virtual void Update(float dt);

private:
    void setupShader();
};

#endif // LIGHTMAP_H
