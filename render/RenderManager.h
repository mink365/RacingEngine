/*
 * RenderSystem.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef RENDERSYSTEM_H_
#define RENDERSYSTEM_H_

#include <camera/Camera.h>
#include <scene/Mesh.h>
#include "RenderQueue.h"
#include "Renderer.h"

class RenderManager {
public:
    RenderManager();
    virtual ~RenderManager();

    Camera &getActivatedCamera();

    void renderMesh(MeshPtr mesh);

    void render();

    void setDefaultRenderEnv();

    void clear();

    RenderQueue &getRenderQueue();

    Renderer &getRenderer();

protected:
    void renderList(RenderableList &list);

    void applyMaterial(Material &material);

    /**
     * @brief initDefaultRenderState
     * 同步硬件/GPU的状态和状态缓冲的状态
     */
    void initDefaultRenderState();

private:
    Camera camera;

    RenderQueue renderQueue;

    Renderer renderer;

    static const RenderState defaultRenderState;
};

#endif /* RENDERSYSTEM_H_ */
