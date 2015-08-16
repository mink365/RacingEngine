#include "Renderable.h"

namespace re {

Renderable::Renderable(const Mat4 &mat, MaterialPtr material, MeshDataPtr data)
{
    this->material = material;
    this->meshData = data;
    this->matrix = mat;
}

} // namespace re

