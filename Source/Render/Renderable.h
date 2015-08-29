#ifndef RE_RENDERABLE_H
#define RE_RENDERABLE_H

#include "Math/Matrix.h"

namespace re {

class Material;
using MaterialPtr = SharedPtr<Material>;
class MeshData;
using MeshDataPtr = SharedPtr<MeshData>;

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
