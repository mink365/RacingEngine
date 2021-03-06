#ifndef MESH_H
#define MESH_H

#include "Base/Named.h"
#include "Base/Clonable.h"
#include "Base/Memory/Buffer.h"
#include "Geometry/Geometry.h"
#include "Material/Material.h"
#include "Render/BufferObject/VertexBuffer.h"
#include "Render/BufferObject/IndexBuffer.h"

namespace re {

class Mesh;
class Geometry;

using MeshPtr = SharedPtr<Mesh>;
using GeometryPtr = SharedPtr<Geometry>;

enum class VertexElementType {
    Position,
    Normal,
    Diffuse,
    Specular,
    // skeleton blend
    BlendWeight,
    BlendIndex,
    // may be more than one(UV2)
    TextureCoord,
    Tangent,
    Binormal,
};

class VertexElement {
public:
    VertexElement(VertexElementType type, AttributeFormat format, int size);

    size_t getElementSize() {
        switch (format) {
        case AttributeFormat::UBYTE:
            return size;
        case AttributeFormat::HALF:
            return 2 * size;
        case AttributeFormat::FLOAT:
            return 4 * size;
        }

        return 0;
    }

public:
    VertexElementType type;
    AttributeFormat format;
    int size;
};

enum class StreamType {
    Geometry,
    Instance,
};

class StreamUnit {
public:
    size_t getVertexSize() {
        size_t size = 0;
        for (auto& element : format) {
            size += element.getElementSize();
        }

        return size;
    }

public:
    StreamType type;
    std::vector<VertexElement> format;

    Buffer<float> vertices;
    VertexBuffer stream;
};

class MeshData
{
public:
    virtual ~MeshData() {}

    std::vector<StreamUnit> vertexStreams;

    Buffer<uint> indices;
    IndexBuffer indexStream;
};

class Mesh : public Shared<Mesh>, public Named
{
public:
    Mesh();

    Geometry::ptr getGeometry();
    void setGeometry(const GeometryPtr &g);

    MeshDataPtr getMeshData() const;
    void setMeshData(MeshDataPtr data);

protected:
    MeshDataPtr data;
    Geometry::ptr geometry;
};

class ControlPoints
{
    friend class SkeletonController;
    friend class FbxParser;

protected:
    void cacheVertex();

protected:
    std::vector<Vec3> controlPoints;
    std::vector<uint> vertexToControl;
    std::vector<std::vector<uint>> controlToVertex;
};

class SkinnedMeshData : public MeshData {
public:
    void setVertex(uint index, const Vec3& position);

public:
    ControlPoints controlPointsData;
};

template<typename T, typename V>
T* Map(Buffer<V>& buf) {
    void* p = buf.getData();

    return static_cast<T*>(p);
}

struct FbxVertex
{
    Vec3 xyz;
    Uv uv;
    Vec3 normal;
    Color color;
};

inline void SkinnedMeshData::setVertex(uint index, const Vec3& position) {
    auto pointer = Map<FbxVertex>(this->vertexStreams[0].vertices);

    pointer[index].xyz = position;
}

} // namespace re

#endif // MESH_H
