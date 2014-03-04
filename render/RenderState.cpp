#include "RenderState.h"

RenderState::RenderState()
{
}

bool RenderState::getDepthTest() const
{
    return depthTest;
}

void RenderState::setDepthTest(bool value)
{
    depthTest = value;
}
