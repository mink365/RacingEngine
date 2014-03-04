#ifndef VERTEX_H
#define VERTEX_H

#include "math/Vector.h"
#include "math/Color.h"
#include "math/UV.h"

class Vertex
{
public:
    Vertex();

    reVec3 xyz;
    reUv uv;
    reVec3 normal;
    reColor color;
};

#endif // VERTEX_H
