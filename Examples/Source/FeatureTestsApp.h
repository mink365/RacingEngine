#ifndef FEATURETESTSAPP_H
#define FEATURETESTSAPP_H

#include "Application.h"
#include "BaseTest.h"
#include "PreDeclare.h"

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

    virtual bool initEnvironment();
    virtual void onEnterForeground();
    virtual void onExitForeground();

protected:
    void createTests();
    void nextTest();
    void lastTest();

    void onCurrentTestChanged();

    void createBaseUI();

    void registerWindows();
    void registerScenes();

private:
    void initResources();

    void update();

private:
    std::vector<BaseTestPtr> tests;
    BaseTestPtr current;
    int currIndex;

    CameraPtr presCamera;
    NodePtr rootNode;
    UIManagerPtr stage;

    ui::LabelPtr labelTitle;
    ui::LabelPtr labelFps;
};

#endif // FEATURETESTSAPP_H
