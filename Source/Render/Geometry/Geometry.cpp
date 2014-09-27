#include "Geometry.h"

#include "Mesh.h"

namespace re {

Geometry::Geometry()
{
}

void Geometry::addVertex(const re::Vertex& v) {
    this->positions.push_back(v.xyz);
    this->uvs.push_back(v.uv);
    this->uv2s.push_back(v.uv2);
    this->vertexNormals.push_back(v.normal);
    this->colors.push_back(v.color);
    this->tangents.push_back(v.tangent);
}

Vertex Geometry::getVertex(uint index)
{
    Vertex v;

    v.xyz = this->positions[index];
    v.uv = this->uvs[index];
    v.uv2 = this->uv2s[index];
    v.normal = this->vertexNormals[index];
    v.color = this->colors[index];
    v.tangent = this->tangents[index];

    return v;
}

void Geometry::addPosition(const Vec3 &v)
{
    this->positions.push_back(v);
}

void Geometry::setPositions(const std::vector<Vec3> &value)
{
    this->positions = value;
}

void Geometry::addFace(const Face &face)
{
    this->faces.push_back(face);
}

void Geometry::setFaces(const std::vector<Face> &value)
{
    faces = value;
}

void Geometry::addUv(const Uv &uv)
{
    this->uvs.push_back(uv);
}

void Geometry::addVertexNormal(const Vec3 &normal)
{
    this->vertexNormals.push_back(normal);
}

void Geometry::appendToMeshData(MeshDataPtr &meshData)
{
    auto& stream = meshData->vertexStreams[0];

    stream.vertices.allocate(this->getVertexCount() * stream.getVertexSize());
    meshData->indices.allocate(this->faces.size() * 3);

    auto vertexPointer = Map<FbxVertex>(stream.vertices);
    auto facePointer = Map<Face>(meshData->indices);

    FbxVertex v;
    for (uint i = 0; i < this->getVertexCount(); i++) {
        v.xyz = this->positions[i];
        v.uv = this->uvs[i];
        v.normal = this->vertexNormals[i];
        v.color = this->colors[i];

        vertexPointer[i] = v;
    }

    for (uint i = 0; i < this->faces.size(); i++) {
        facePointer[i] = this->faces[i];
    }
}

void Geometry::clear()
{
    this->positions.clear();
    this->uvs.clear();
    this->uv2s.clear();
    this->vertexNormals.clear();
    this->tangents.clear();
    this->colors.clear();

    this->normals.clear();
    this->faces.clear();
}

// TODO: no clone needed
Geometry::ptr Geometry::clone() const
{
    Geometry::ptr inst = Geometry::create();

//    inst->vertices = this->vertices;
    inst->faces = this->faces;

    // vbo can't be copy, clone a geometry always means a new vbo(hardware ID)

    return inst;
}

Face::Face(uint a, uint b, uint c)
    :a(a), b(b), c(c)
{
}

}
