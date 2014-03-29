#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "platform.h"

namespace re {

class VertexBuffer
{
public:
    VertexBuffer();

    uint vbo;
    uint size;
};

} // namespace re

#endif // VERTEXBUFFER_H
