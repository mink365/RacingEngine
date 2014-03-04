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

    void addRootNode(SceneNode *node);
    void removeRootNode(const SceneNode *node);
    void clearRootNodes();
    std::vector<SceneNode *> &getRootNodes();

    void renderScene();

    RenderManager &getRenderManager();

    /**
     * @brief clearFrame
     * 清除绘制时产生的临时数据
     */
    void clearFrame();

protected:
    void vist(SceneNode &node);

private:
    SceneManager();

private:
    std::vector<SceneNode*> roots;

    std::vector<SceneNode*> sceneTransformUpdateBucket;

    RenderManager renderManger;
};

#endif // SCENEMANAGER_H
