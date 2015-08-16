#ifndef RE_RENDERABLE_H
#define RE_RENDERABLE_H

#include "PreDeclare.h"
#include "Math/Matrix.h"

namespace re {

class Renderable
{
public:
    Renderable(const Mat4& mat, MaterialPtr material, MeshDataPtr data);

public:
    Mat4 matrix;
    MaterialPtr material;
    MeshDataPtr meshData;
};

} // namespace re

#endif // RE_RENDERABLE_H
