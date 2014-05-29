#ifndef VERTEX_H
#define VERTEX_H

#include "Math/Vector.h"
#include "Math/Color.h"
#include "Math/UV.h"

namespace re {

class Vertex
{
public:
    Vertex();

    Vec3 xyz;
    Uv uv;
    Vec3 normal;
    Color color;
};

} // namespace re

#endif // VERTEX_H
