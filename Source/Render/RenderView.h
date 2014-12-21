#ifndef RE_RENDERVIEW_H
#define RE_RENDERVIEW_H

#include <functional>
#include "Base/Shared.h"
#include "PreDeclare.h"
#include "Math/Matrix.h"
#include "Math/Rect.h"
#include "Math/Color.h"
#include "Scene/Camera/Camera.h"

namespace re {

class Shader;
class RenderTarget;
class ShadowInfo;

class RenderView : public Shared<RenderView>
{
public:
    RenderView();

    void init(CameraPtr& camera);
    void init(LightPtr& light);

    void setupShadowInfo(const ShadowInfo &shadowInfo);

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
    Color _clearColor;

    std::function<bool(int queueID)> queueCullFunc;

    std::shared_ptr<Shader> forceShader;
    std::shared_ptr<RenderTarget> renderTarget;
};

} // namespace re

#endif // RE_RENDERVIEW_H
