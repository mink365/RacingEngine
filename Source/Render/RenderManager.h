/*
 * RenderSystem.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef RENDERSYSTEM_H_
#define RENDERSYSTEM_H_

#include "Camera/Camera.h"
#include "Scene/Mesh.h"
#include "RenderQueue.h"
#include "Renderer.h"

namespace re {

class RenderManager {
public:
    RenderManager();
    virtual ~RenderManager();

    void addCamera(CameraPtr camera);

    void render();

    void setDefaultRenderEnv();

    void clear();

    RenderQueue &getRenderQueue();

    Renderer &getRenderer();

protected:
    void renderList(const RenderableList &list);

    void renderAttribute(const NodeAttributePtr &attribute);
    void renderMesh(const MeshPtr &mesh);

    void applyMaterial(Material &material);

    /**
     * @brief initDefaultRenderState
     * 同步硬件/GPU的状态和状态缓冲的状态
     */
    void initDefaultRenderState();

    void activeCamera(CameraPtr camera);

private:
    std::vector<CameraPtr> cameraList;
    CameraPtr currCamera;

    RenderQueue renderQueue;

    Renderer renderer;

    static const RenderState defaultRenderState;
};

} // namespace re

#endif /* RENDERSYSTEM_H_ */
