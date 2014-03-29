#include "Geometry.h"

namespace re {

Geometry::Geometry()
{
}

void Geometry::addVertex(Vertex &v)
{
    this->vertices.push_back(v);
}

void Geometry::addFace(Face &face)
{
    this->faces.push_back(face);
}

std::vector<Vertex> &Geometry::getVertices()
{
    return vertices;
}

void Geometry::setVertices(const std::vector<Vertex> &value)
{
    vertices = value;
}

std::vector<Face> &Geometry::getFaces()
{
    return faces;
}

void Geometry::setFaces(const std::vector<Face> &value)
{
    faces = value;
}

IndexBuffer &Geometry::getIbo()
{
    return ibo;
}

VertexBuffer &Geometry::getVbo()
{
    return vbo;
}

}
