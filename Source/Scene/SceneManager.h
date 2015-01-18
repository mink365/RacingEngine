#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
#include "Base/Singleton.h"

#include "Scene/Node.h"
#include "Render/RenderManager.h"

namespace re {

using std::vector;

class SceneManager : public Singleton<SceneManager>
{
    friend class Singleton;

public:
    void addRootNode(NodePtr node);
    void removeRootNode(const NodePtr node);
    void clearRootNodes();

    void renderScene();

    RenderManager &getRenderManager();

    /**
     * @brief clearFrame
     * 清除绘制时产生的临时数据
     */
    void clearFrame();

protected:
    void vist(const NodePtr& node);

private:
    SceneManager();

private:
    NodePtr root;

    std::vector<NodePtr> sceneTransformUpdateBucket;

    RenderManager renderManger;
};

} // namespace re

#endif // SCENEMANAGER_H
