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
#include "RenderView.h"
#include "Renderable.h"

namespace re {

class Renderer;
using RendererPtr = std::shared_ptr<Renderer>;

class RenderManager {
public:
    RenderManager();
    virtual ~RenderManager();

    void addCamera(CameraPtr& camera);
    void addLight(LightPtr& light);

    void render();

    void clear();

    RenderQueue &getRenderQueue();

    void setRenderer(RendererPtr &renderer);
    Renderer &getRenderer();

    /**
     * @brief initDefaultRenderState
     * sync hw state and engine context
     */
    void initDefaultRenderState();

    void markRenderViewDirty();

public:
    void createRenderViews();

    void renderList(const RenderableList &list);

    void renderElement(const Renderable& element);

    void applyMaterial(Material &material);

    void activeRenderView(RenderView::ptr &view);

public:
    std::vector<CameraPtr> cameraList;
    std::vector<LightPtr> lightList;

    bool renderViewDirty;

    std::vector<RenderView::ptr> renderViewList;
    RenderView::ptr currRenderView;

    RenderQueue renderQueue;

    RendererPtr renderer;

    static const RenderState defaultRenderState;
};

} // namespace re

#endif /* RENDERSYSTEM_H_ */
