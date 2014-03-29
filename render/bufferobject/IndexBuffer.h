#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "platform.h"

namespace re {

class IndexBuffer
{
public:
    IndexBuffer();

    uint vboIB;
    uint nIndices;
    uint indexSize;
};

} // namespace re

#endif // INDEXBUFFER_H
