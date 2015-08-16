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
    std::vector<std::shared_ptr<BaseTest>> tests;
    std::shared_ptr<BaseTest> current;
    int currIndex;

    CameraPtr presCamera;
    NodePtr rootNode;
    std::shared_ptr<ui::UIManager> stage;

    ui::LabelPtr labelTitle;
    ui::LabelPtr labelFps;
};

#endif // FEATURETESTSAPP_H
