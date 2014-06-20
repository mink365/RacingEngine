#include "Geometry.h"

namespace re {

Geometry::Geometry()
    : staticGeometry(true), dirtyFlag(true)
{
}

void Geometry::addVertex(const Vertex &v)
{
    this->vertices.push_back(v);
}

void Geometry::addFace(const Face &face)
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

Geometry::ptr Geometry::clone() const
{
    Geometry::ptr inst = Geometry::create();

    inst->controlPointsData = this->controlPointsData;
    inst->vertices = this->vertices;
    inst->faces = this->faces;

    // vbo can't be copy, clone a geometry always means a new vbo(hardware ID)

    inst->dirtyFlag = true;
    inst->staticGeometry = this->staticGeometry;

    return inst;
}

}
