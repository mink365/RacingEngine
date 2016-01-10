#ifndef FEATURETESTSAPP_H
#define FEATURETESTSAPP_H

#include "Application.h"
#include "BaseTest.h"
#include "PreDeclare.h"
#include "Base/ECS/Event.h"
#include "Base/ECS/System.h"
#include "Base/ECS/EntityManager.h"

#include <vector>
#include <memory>

using namespace re;

namespace re {
namespace ui {
    class UIManager;
}
}

using BaseTestPtr = SharedPtr<BaseTest>;

class FeatureTestsApp : public Application
{
    friend class BaseTest;

public:
    FeatureTestsApp();

    virtual bool initEnvironment() override;
    virtual void onEnterForeground() override;
    virtual void onExitForeground() override;

protected:
    void createTests();
    void nextTest();
    void lastTest();

    void onCurrentTestChanged();

    void createBaseUI();

    void registerWindows();
    void registerScenes();

private:
    void initGame();
    void initResources();

    void update();

private:
    std::vector<BaseTestPtr> tests;
    BaseTestPtr current;
    size_t currIndex;

    CameraPtr presCamera;
    NodePtr rootNode;
    UIManagerPtr stage;

    ui::TextPtr labelTitle;
    ui::TextPtr labelFps;
};

#endif // FEATURETESTSAPP_H
