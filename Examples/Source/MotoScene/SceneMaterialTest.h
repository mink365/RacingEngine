#ifndef SCENEMATERIALTEST_H
#define SCENEMATERIALTEST_H

#include "BaseTest.h"

class SceneMaterialTest : public BaseTest
{
public:
    SceneMaterialTest();

    virtual void Init();
    virtual void Update();

private:
    void createUI();
    void changeTile(size_t index);

private:
    std::vector<NodePtr> tiles;
    std::size_t tileIndex;
};

#endif // SCENEMATERIALTEST_H
