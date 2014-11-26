#ifndef FLATRENDERER_H
#define FLATRENDERER_H

#include <vector>

#include "Render/RenderState.h"

class FlatRenderer
{
public:
    FlatRenderer();

private:
    std::vector<re::RenderState> renderStateStack;
};

#endif // FLATRENDERER_H
