#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include "Vertex.h"
#include "Face.h"
#include <render/bufferobject/VertexBuffer.h>
#include <render/bufferobject/IndexBuffer.h>

class Geometry
{
    friend class Renderer;
    friend class BufferObjectUtil;

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
    std::vector<Vertex> vertices;
    std::vector<Face> faces;

    VertexBuffer vbo;
    IndexBuffer ibo;
};

#endif // GEOMETRY_H
