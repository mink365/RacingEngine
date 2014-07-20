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

    void clear();

    RenderQueue &getRenderQueue();

    Renderer &getRenderer();

    /**
     * @brief initDefaultRenderState
     * sync hw state and engine context
     */
    void initDefaultRenderState();

protected:
    void renderList(const RenderableList &list);

    void renderAttribute(const NodeAttributePtr &attribute);
    void renderMesh(const MeshPtr &mesh);

    void applyMaterial(Material &material);

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
