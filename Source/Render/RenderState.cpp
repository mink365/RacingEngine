#include "RenderState.h"

namespace re {

RenderState::RenderState()
{
    this->blendState.blendEnable = true;
    this->blendState.blendModeAlpha = BlendMode::Alpha;
    this->blendState.blendModeRGB = BlendMode::Alpha;

    this->depthState.depthTestEnable = true;
    this->depthState.depthWrite = true;
    this->depthState.function = TestFunction::LessOrEqual;

    this->alphaState.alphaTestEnable = false;
    this->alphaState.alphaFallOff = 0;
    this->alphaState.function = TestFunction::LessOrEqual;

    this->faceCullState.faceCullEnable = true;
    this->faceCullState.cullMode = FaceCullMode::Back;

    this->stencilState.stencilTestEnable = false;
    this->stencilState.frontStencilStencilFailOperation = StencilOperation::Keep;
    this->stencilState.frontStencilDepthFailOperation = StencilOperation::Keep;
    this->stencilState.frontStencilDepthPassOperation = StencilOperation::Keep;
    this->stencilState.backStencilStencilFailOperation = StencilOperation::Keep;
    this->stencilState.backStencilDepthFailOperation = StencilOperation::Keep;
    this->stencilState.backStencilDepthPassOperation = StencilOperation::Keep;
    this->stencilState.frontStencilFunction = TestFunction::Always;
    this->stencilState.backStencilFunction = TestFunction::Always;
}

bool DepthState::operator==(const DepthState &right) const
{
    return (this->depthTestEnable == right.depthTestEnable
            && this->depthWrite == right.depthWrite
            && this->function == right.function
            );
}

bool DepthState::operator!=(const DepthState &right) const
{
    return !(*this == right);
}

bool StencilState::operator==(const StencilState &right) const
{
    return (this->stencilTestEnable == right.stencilTestEnable
            && this->backStencilDepthFailOperation == right.backStencilDepthFailOperation
            && this->backStencilDepthPassOperation == right.backStencilDepthPassOperation
            && this->backStencilFunction == right.backStencilFunction
            && this->backStencilStencilFailOperation == right.backStencilStencilFailOperation
            && this->frontStencilDepthFailOperation == right.frontStencilDepthFailOperation
            && this->frontStencilDepthPassOperation == right.frontStencilDepthPassOperation
            && this->frontStencilFunction == right.frontStencilFunction
            && this->frontStencilStencilFailOperation == right.frontStencilStencilFailOperation
            );
}

bool StencilState::operator!=(const StencilState &right) const
{
    return  !(this->stencilTestEnable == false && right.stencilTestEnable == false)
            && !(*this == right);
}

bool AlphaState::operator==(const AlphaState &right) const
{
    return (this->alphaTestEnable == right.alphaTestEnable
            && this->function == right.function
            && this->alphaFallOff == right.alphaFallOff
            );
}

bool AlphaState::operator!=(const AlphaState &right) const
{
    return  !(this->alphaTestEnable == false && right.alphaTestEnable == false)
            && !(*this == right);
}

bool FaceCullState::operator==(const FaceCullState &right) const
{
    return (this->faceCullEnable == right.faceCullEnable
            && this->cullMode == right.cullMode
            );
}

bool FaceCullState::operator!=(const FaceCullState &right) const
{
    return  !(this->faceCullEnable == false && right.faceCullEnable == false)
            && !(*this == right);
}

bool BlendState::operator==(const BlendState &right) const
{
    return (this->blendEnable == right.blendEnable
            && this->blendModeRGB == right.blendModeRGB
            && this->blendModeAlpha == right.blendModeAlpha);
}

bool BlendState::operator!=(const BlendState &right) const
{
    return !(this->blendEnable == false && right.blendEnable == false)
            && !(*this == right);
}

}
