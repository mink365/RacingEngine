#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "platform.h"

class IndexBuffer
{
public:
    IndexBuffer();

    uint vboIB;
    uint nIndices;
    uint indexSize;
};

#endif // INDEXBUFFER_H
