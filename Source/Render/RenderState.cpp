#include "RenderState.h"

namespace re {

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

}
