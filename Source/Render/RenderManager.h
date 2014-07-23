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

namespace re {

class Renderer;

class RenderManager {
public:
    RenderManager();
    virtual ~RenderManager();

    void addCamera(CameraPtr camera);

    void render();

    void clear();

    RenderQueue &getRenderQueue();

    void setRenderer(std::shared_ptr<Renderer> &renderer);
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

    std::shared_ptr<Renderer> renderer;

    static const RenderState defaultRenderState;
};

} // namespace re

#endif /* RENDERSYSTEM_H_ */
