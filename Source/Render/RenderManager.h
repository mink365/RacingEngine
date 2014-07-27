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
#include "Render/Renderer/Renderer.h"

namespace re {

class Renderer;

class RenderManager {
public:
    RenderManager();
    virtual ~RenderManager();

    void addCamera(CameraPtr& camera);
    void addLight(LightPtr& light);

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
    void createRenderViews();

    void renderList(const RenderableList &list);

    void renderAttribute(const NodeAttributePtr &attribute);
    void renderMesh(const MeshPtr &mesh);

    void applyMaterial(Material &material);

    void activeRenderView(RenderView::ptr &view);

public:
    std::vector<CameraPtr> cameraList;
    std::vector<LightPtr> lightList;

    bool renderViewDirty;

    std::vector<RenderView::ptr> renderViewList;
    RenderView::ptr currRenderView;

    RenderQueue renderQueue;

    std::shared_ptr<Renderer> renderer;

    static const RenderState defaultRenderState;
};

} // namespace re

#endif /* RENDERSYSTEM_H_ */
