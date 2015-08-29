#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <array>
#include <functional>

#include "Math/Vector.h"
#include "Math/Color.h"
#include "Math/UV.h"

#include "Base/Shared.h"
#include "Base/Clonable.h"

namespace re {

class MeshData;
using MeshDataPtr = SharedPtr<MeshData>;

class Vertex {
public:
    Vec3 xyz;
    Uv uv;
    Uv uv2;
    Vec3 normal;
    Vec4 tangent;
    Color color;
};

class Face
{
public:
    uint a;
    uint b;
    uint c;

    Face(uint a, uint b, uint c);
};

/**
 * @brief CalculateTangents
 * @param positions
 * @param vertexNormals
 * @param uvs
 * @param faces
 * @param tangents
 *
 * http://www.terathon.com/code/tangent.html
 *
 * http://blog.db-in.com/calculating-normals-and-tangent-space/
 */
void CalculateTangents(std::vector<Vec3>& positions, std::vector<Vec3>& vertexNormals, std::vector<Uv>& uvs, std::vector<Face>& faces, std::vector<Vec4>& tangents);

class Geometry : public Shared<Geometry>, public Clonable<Geometry>
{
    friend class SkeletonController;
    friend class FbxParser;

public:
    Geometry();

    void addVertex(const Vertex& v);
    uint getVertexCount();
    Vertex getVertex(uint index);

    void addPosition(const Vec3 &v);
    std::vector<Vec3>& getPositions();
    void setPositions(const std::vector<Vec3>& value);

    void addFace(const Face &face);
    std::vector<Face> &getFaces();
    void setFaces(const std::vector<Face> &value);

    void addUv(const Uv& uv);
    void addVertexNormal(const Vec3& normal);
    void addVertexUvColor(const Vec3& v, const Uv& uv, const Color& color);

    std::vector<Uv>& getUvs();
    std::vector<Vec3>& getVertexNormals();
    std::vector<Color>& getDiffuseColors();

    void calculateTangents();

    void appendToMeshData(MeshDataPtr& meshData);
    void setMeshDataAppendFunc(std::function<void(Geometry& geometry, MeshData& meshData)> func);

    void clear();

    Geometry::ptr clone() const override;

public:
    std::vector<Vec3> positions;
    std::vector<Vec3> vertexNormals;
    std::vector<Color> colors;
    std::vector<Uv> uvs;
    std::vector<Uv> uv2s;
    std::vector<Vec4> tangents;

    // surface normal
    std::vector<Vec3> normals;
    std::vector<Face> faces;

    std::function<void(Geometry& geometry, MeshData& meshData)> appendFunc;
};

inline uint Geometry::getVertexCount()
{
    return this->positions.size();
}

inline std::vector<Vec3> &Geometry::getPositions()
{
    return positions;
}

inline std::vector<Uv>& Geometry::getUvs() {
    return this->uvs;
}

inline std::vector<Vec3>& Geometry::getVertexNormals() {
    return this->vertexNormals;
}

inline std::vector<Color>& Geometry::getDiffuseColors() {
    return this->colors;
}

inline std::vector<Face> &Geometry::getFaces()
{
    return faces;
}

} // namespace re

#endif // GEOMETRY_H
