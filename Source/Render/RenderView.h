#ifndef RE_RENDERVIEW_H
#define RE_RENDERVIEW_H

#include <functional>
#include "Base/Shared.h"
#include "PredefineType.h"
#include "Math/Matrix.h"
#include "Math/BoundingVolume.h"
#include "Scene/Camera/Camera.h"

namespace re {

class Shader;
class RenderTarget;

class RenderView : public Shared<RenderView>
{
public:
    RenderView();

    void init(CameraPtr& camera);
    void init(LightPtr& light);

public:
    CameraProjectionMode mode;

    Rect viewport;

    Mat4 viewMatrix;
    Mat4 projMatrix;

    float zNear;
    float zFar;

    union {
        float fov;
        float orthoWidth;
    };

    int clearFlag;
    std::function<bool(int queueID)> queueCullFunc;

    std::shared_ptr<Shader> forceShader;
    std::shared_ptr<RenderTarget> renderTarget;
};

} // namespace re

#endif // RE_RENDERVIEW_H
