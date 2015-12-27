#include "RenderState.h"

namespace re {

RenderState::RenderState()
{
    this->blendState.blendEnable = true;
    this->blendState.blendModeAlpha = BlendMode::PremultAlpha;
    this->blendState.blendModeRGB = BlendMode::PremultAlpha;

    this->depthState.depthTestEnable = true;
    this->depthWrite = true;
    this->depthState.function = TestFunction::LessOrEqual;

    this->alphaState.alphaTestEnable = false;
    this->alphaState.alphaFallOff = 0;
    this->alphaState.function = TestFunction::LessOrEqual;

    this->faceCullMode = FaceCullMode::Off;
    this->polygonMode = PolygonMode::Fill;

    this->stencilState.testEnable = false;
}

bool DepthTestState::operator==(const DepthTestState &right) const
{
    return (this->depthTestEnable == right.depthTestEnable
            && this->function == right.function
            );
}

bool DepthTestState::operator!=(const DepthTestState &right) const
{
    return !(*this == right);
}

bool StencilState::operator==(const StencilState &right) const
{
    return (this->testEnable == right.testEnable
            && this->refValue == right.refValue
            && this->function == right.function
            && this->failOperation == right.failOperation
            && this->depthFailOperation == right.depthFailOperation
            && this->depthPassOperation == right.depthPassOperation
            );
}

bool StencilState::operator!=(const StencilState &right) const
{
    return  !(this->testEnable == false && right.testEnable == false)
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
