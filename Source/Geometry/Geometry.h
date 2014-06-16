#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <array>
#include "Vertex.h"
#include "Face.h"
#include "Render/BufferObject/VertexBuffer.h"
#include "Render/BufferObject/IndexBuffer.h"

namespace re {

class Geometry
{
    friend class Renderer;
    friend class BufferObjectUtil;
    friend class SkeletonController;

public:
    Geometry();

    void addVertex(Vertex &v);
    void addFace(Face &face);

    std::vector<Vertex> &getVertices();
    void setVertices(const std::vector<Vertex> &value);

    std::vector<Face> &getFaces();
    void setFaces(const std::vector<Face> &value);

    VertexBuffer &getVbo();

    IndexBuffer &getIbo();

private:
    void chacheVertex();

private:
    std::vector<Vertex> vertices;
    std::vector<Face> faces;

    std::vector<Vertex> controlPoints;
    std::vector<int> vertexToControl;
    std::vector<vector<int>> controlToVertex;

    VertexBuffer vbo;
    IndexBuffer ibo;
};

} // namespace re

#endif // GEOMETRY_H
