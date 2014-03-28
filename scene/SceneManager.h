#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
#include "SceneNode.h"
#include <render/RenderManager.h>

using std::vector;

class SceneManager
{
public:
    static SceneManager &getInstance();

    void addRootNode(SceneNodePtr node);
    void removeRootNode(const SceneNodePtr node);
    void clearRootNodes();
    std::vector<SceneNodePtr> &getRootNodes();

    void renderScene();

    RenderManager &getRenderManager();

    /**
     * @brief clearFrame
     * 清除绘制时产生的临时数据
     */
    void clearFrame();

protected:
    void vist(SceneNodePtr node);

private:
    SceneManager();

private:
    std::vector<SceneNodePtr> roots;

    std::vector<SceneNodePtr> sceneTransformUpdateBucket;

    RenderManager renderManger;
};

#endif // SCENEMANAGER_H
