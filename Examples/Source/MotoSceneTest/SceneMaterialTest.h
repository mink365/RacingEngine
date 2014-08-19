#ifndef SCENEMATERIALTEST_H
#define SCENEMATERIALTEST_H

#include "BaseTest.h"

class SceneMaterialTest : public BaseTest
{
public:
    SceneMaterialTest();

    virtual void Init();
    virtual void Update(float dt);

private:
    void createUI();
    void changeTile(int index);

private:
    std::vector<SceneNodePtr> tiles;
    int tileIndex;
};

#endif // SCENEMATERIALTEST_H
