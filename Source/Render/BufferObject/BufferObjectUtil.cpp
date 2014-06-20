#include "BufferObjectUtil.h"

namespace re {

BufferObjectUtil::BufferObjectUtil()
{
}

void BufferObjectUtil::loadGeometryToHardware(Geometry &geometry)
{
    int face_count = geometry.getFaces().size();

    GLushort    pindex_buffer[face_count * 3];
    for (int i = 0; i < face_count; ++i) {
        Face face = geometry.getFaces()[i];

        pindex_buffer[i * 3 + 0] = face.a;
        pindex_buffer[i * 3 + 1] = face.b;
        pindex_buffer[i * 3 + 2] = face.c;
    }

    geometry.ibo.nIndices = face_count * 3;
    geometry.ibo.indexSize = face_count * 3 *sizeof(GLushort);

    GLint mode;
    if (geometry.isStatic()) {
        mode = GL_STATIC_DRAW;
    } else {
        mode = GL_DYNAMIC_DRAW;
    }

    glGenBuffers(1, &(geometry.ibo.vboIB));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.ibo.vboIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.ibo.indexSize, pindex_buffer, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    int vertex_count = geometry.getVertices().size();
    geometry.vbo.size = vertex_count *sizeof(Vertex);

    glGenBuffers(1, &(geometry.vbo.vbo));
    glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo.vbo);
    glBufferData(GL_ARRAY_BUFFER, geometry.vbo.size, (GLvoid*)geometry.getVertices().data(), mode);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BufferObjectUtil::updateGeometryToHardware(Geometry &geometry)
{
    glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, geometry.vbo.size, (GLvoid*)geometry.getVertices().data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
