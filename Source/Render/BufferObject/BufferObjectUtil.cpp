#include "BufferObjectUtil.h"
#include "Scene/Mesh.h"

#include "opengl.h"

namespace re {

BufferObjectUtil::BufferObjectUtil()
{
}

void BufferObjectUtil::loadGeometryToHardware(MeshData &meshData)
{
    // transform from uint to short
    int index_count = meshData.indices.getSize();

    auto indexPointer = Map<uint>(meshData.indices);
    GLushort short_index_buffer[index_count];
    for (int i = 0; i < index_count; ++i) {
        short_index_buffer[i] = indexPointer[i];
    }

    meshData.indexStream.nIndices = index_count;
    meshData.indexStream.indexSize = index_count * sizeof(GLushort);

    GLint mode;
//    if (geometry.isStatic()) {
//        mode = GL_STATIC_DRAW;
//    } else {
        mode = GL_DYNAMIC_DRAW;
//    }

    if (meshData.indexStream.vboIB == 0) {
        glGenBuffers(1, &(meshData.indexStream.vboIB));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData.indexStream.vboIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indexStream.indexSize, short_index_buffer, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    for (auto& unit : meshData.vertexStreams) {
        VertexBuffer& stream = unit.stream;

        stream.size = unit.vertices.getByteSize();

        if (stream.vbo == 0) {
            glGenBuffers(1, &(stream.vbo));
        }

        glBindBuffer(GL_ARRAY_BUFFER, stream.vbo);
        glBufferData(GL_ARRAY_BUFFER, stream.size, (GLvoid*)unit.vertices.getData(), mode);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void BufferObjectUtil::updateGeometryToHardware(const MeshData &meshData)
{
    for (auto& unit : meshData.vertexStreams) {
        const VertexBuffer& stream = unit.stream;

        glBindBuffer(GL_ARRAY_BUFFER, stream.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, stream.size, (GLvoid*)unit.vertices.getData());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

}
