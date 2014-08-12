#ifndef FEATURETESTSAPP_H
#define FEATURETESTSAPP_H

#include "Application.h"
#include "BaseTest.h"
#include "PredefineType.h"

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
private:
    void initResources();

    void update(long dt);

private:
    std::vector<std::shared_ptr<BaseTest>> tests;
    std::shared_ptr<BaseTest> current;
    int currIndex;

    CameraPtr presCamera;
    SceneNodePtr rootNode;
    std::shared_ptr<ui::UIManager> stage;

    LabelPtr labelTitle;
    LabelPtr labelFps;
};

class WindowFactory : public IWindowFactory
{
public:
    virtual std::shared_ptr<Window> createView(const string& name) override;
};

class SceneFactory : public ISceneFactory
{
public:
    virtual std::shared_ptr<Scene> createView(const string& name) override;
};

#endif // FEATURETESTSAPP_H
