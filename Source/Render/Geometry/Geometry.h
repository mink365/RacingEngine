#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <array>
#include "Base/Shared.h"
#include "Base/Clonable.h"
#include "Vertex.h"
#include "Face.h"
#include "ControlPoints.h"
#include "Render/BufferObject/VertexBuffer.h"
#include "Render/BufferObject/IndexBuffer.h"

namespace re {

class Geometry : public Shared<Geometry>, public Clonable<Geometry>
{
    friend class Renderer;
    friend class GLES2Renderer;
    friend class BufferObjectUtil;
    friend class SkeletonController;
    friend class FbxParser;

public:
    Geometry();

    void addVertex(const Vertex &v);
    void addFace(const Face &face);

    std::vector<Vertex> &getVertices();
    void setVertices(const std::vector<Vertex> &value);

    std::vector<Face> &getFaces();
    void setFaces(const std::vector<Face> &value);

    void clear();

    VertexBuffer &getVbo();
    IndexBuffer &getIbo();

    bool isDirty() const;
    void setDirty();

    /**
     * @brief isStatic
     * @return
     *
     * this is a static geometry or not.static geometry cann't be modify in runtime.
     */
    bool isStatic() const;

    bool isLoaded() const;

    Geometry::ptr clone() const override;

private:
    ControlPoints controlPointsData;

    std::vector<Vertex> vertices;
    std::vector<Face> faces;

    VertexBuffer vbo;
    IndexBuffer ibo;

    bool dirtyFlag;
    bool staticGeometry;
};

inline bool Geometry::isDirty() const
{
    return this->dirtyFlag;
}

inline void Geometry::setDirty()
{
    this->dirtyFlag = true;
}

inline bool Geometry::isStatic() const
{
    return this->staticGeometry;
}

inline bool Geometry::isLoaded() const
{
    return this->vbo.vbo != 0;
}

} // namespace re

#endif // GEOMETRY_H
