#include "Geometry.h"

#include "Mesh.h"

namespace re {

Geometry::Geometry()
{
    // default append func
    appendFunc = [](Geometry& geometry, MeshData& meshData) {
        auto& stream = meshData.vertexStreams[0];

        stream.vertices.allocate(geometry.getVertexCount() * stream.getVertexSize());
        meshData.indices.allocate(geometry.faces.size() * 3);

        auto vertexPointer = Map<FbxVertex>(stream.vertices);
        auto facePointer = Map<Face>(meshData.indices);

        FbxVertex v;
        for (uint i = 0; i < geometry.getVertexCount(); i++) {
            v.xyz = geometry.positions[i];
            v.uv = geometry.uvs[i];
            v.normal = geometry.vertexNormals[i];
            v.color = geometry.colors[i];

            vertexPointer[i] = v;
        }

        for (uint i = 0; i < geometry.faces.size(); i++) {
            facePointer[i] = geometry.faces[i];
        }
    };
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

void Geometry::calculateTangents()
{
    CalculateTangents(this->positions, this->vertexNormals, this->uvs, this->faces, this->tangents);
}

void Geometry::appendToMeshData(MeshDataPtr &meshData)
{
    this->appendFunc(*this, *meshData);
}

void Geometry::setMeshDataAppendFunc(std::function<void (Geometry &, MeshData &)> func)
{
    this->appendFunc = func;
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

void CalculateTangents(std::vector<Vec3>& positions, std::vector<Vec3>& vertexNormals, std::vector<Uv>& uvs, std::vector<Face>& faces, std::vector<Vec4>& tangents) {
    std::vector<Vec3> tan1(positions.size());
    std::vector<Vec3> tan2(positions.size());

    for (auto& face : faces) {
        Vec3& v1 = positions[face.a];
        Vec3& v2 = positions[face.b];
        Vec3& v3 = positions[face.c];

        Uv& w1 = uvs[face.a];
        Uv& w2 = uvs[face.b];
        Uv& w3 = uvs[face.c];

        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;

        float s1 = w2.u - w1.u;
        float s2 = w3.u - w1.u;
        float t1 = w2.v - w1.v;
        float t2 = w3.v - w1.v;

        float r = 1.0F / (s1 * t2 - s2 * t1);
        Vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                  (t2 * z1 - t1 * z2) * r);
        Vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                  (s1 * z2 - s2 * z1) * r);

        tan1[face.a] += sdir;
        tan1[face.b] += sdir;
        tan1[face.c] += sdir;

        tan2[face.a] += tdir;
        tan2[face.b] += tdir;
        tan2[face.c] += tdir;
    }

    for (size_t i=0; i < positions.size(); ++i) {
        Vec3& n = vertexNormals[i];
        Vec3& t = tan1[i];

        // Gram-Schmidt orthogonalize
        tangents[i] = (t - n * (n * t)).normalize();

        // Calculate handedness
        tangents[i].w = ((n.cross(t) * tan2[i]) < 0.0F) ? -1.0F : 1.0F;
    }
}

}
