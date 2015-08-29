#ifndef RE_RENDERVIEW_H
#define RE_RENDERVIEW_H

#include <functional>
#include "Base/Shared.h"
#include "Math/Matrix.h"
#include "Math/Rect.h"
#include "Math/Color.h"
#include "Scene/Camera/Camera.h"

namespace re {

class Shader;
class RenderTarget;
class ShadowInfo;
class Light;

using LightPtr = SharedPtr<Light>;
using ShaderPtr = SharedPtr<Shader>;

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

    ShaderPtr forceShader;
    RenderTargetPtr renderTarget;
};

} // namespace re

#endif // RE_RENDERVIEW_H
