#ifndef MESH_H
#define MESH_H

#include "SceneNode.h"
#include "Geometry/Geometry.h"
#include "Material/Material.h"
#include "NodeAttribute.h"
#include "Render/BufferObject/VertexBuffer.h"
#include "Render/BufferObject/IndexBuffer.h"

namespace re {

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

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
    size_t getElementSize() {
        switch (format) {
        case AttributeFormat::UBYTE:
            return size;
        case AttributeFormat::HALF:
            return 2 * size;
        case AttributeFormat::FLOAT:
            return 4 * size;
        }
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
    VertexBuffer stream;

    uint32_t count;
};

class MeshData : public Shared<MeshData>
{
public:
    virtual ~MeshData() {};

    Buffer<float> vertices;
    Buffer<unsigned short> indices;

    std::vector<StreamUnit> vertexStreams;

    IndexBuffer indexStream;
};

class Mesh : public NodeAttribute
{
public:
    Mesh();

    /**
     * @brief init
     * init a material and geometry
     */
    void init();

    Material::ptr getMaterial();
    void setMaterial(MaterialPtr& m);

    Geometry::ptr getGeometry();
    void setGeometry(GeometryPtr& g);

    MeshData::ptr getMeshData() const;

    MeshPtr clone();

protected:
    virtual NodeAttributePtr createCloneInstance() const override;
    virtual void copyProperties(const NodeAttribute* att) override;

protected:
    Material::ptr material;

    MeshData::ptr data;
    Geometry::ptr geometry;
};

inline MeshData::ptr Mesh::getMeshData() const
{
    return this->data;
}

class ControlPoints
{
    friend class SkeletonController;
    friend class FbxParser;

public:
    ControlPoints() {};

protected:
    void cacheVertex();

protected:
    std::vector<Vec3> controlPoints;
    std::vector<int> vertexToControl;
    std::vector<std::vector<int>> controlToVertex;
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
    auto pointer = Map<FbxVertex>(this->vertices);

    pointer[index].xyz = position;
}

} // namespace re

#endif // MESH_H
