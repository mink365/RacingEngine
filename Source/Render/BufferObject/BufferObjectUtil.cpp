#include "BufferObjectUtil.h"
#include "Scene/Mesh.h"

#include "opengl.h"

namespace re {

BufferObjectUtil::BufferObjectUtil()
{
}

void BufferObjectUtil::loadGeometryToHardware(Mesh &mesh)
{
    GeometryPtr geometry = mesh.getGeometry();
    MeshDataPtr meshData = mesh.getMeshData();

    if (!meshData) {
        meshData = std::make_shared<MeshData>();
        mesh.setMeshData(meshData);

        StreamUnit unit;
        unit.format.push_back(VertexElement(VertexElementType::Position, AttributeFormat::FLOAT, 3));
        unit.format.push_back(VertexElement(VertexElementType::TextureCoord, AttributeFormat::FLOAT, 2));
        unit.format.push_back(VertexElement(VertexElementType::Normal, AttributeFormat::FLOAT, 3));
        unit.format.push_back(VertexElement(VertexElementType::Diffuse, AttributeFormat::FLOAT, 4));
        meshData->vertexStreams.push_back(unit);
    }

    if (geometry) {
        geometry->appendToMeshData(meshData);
    }

    // transform from uint to short
    int face_count = meshData->indices.getSize() / 3;

    auto facePointer = Map<Face>(meshData->indices);
    GLushort    pindex_buffer[face_count * 3];
    for (int i = 0; i < face_count; ++i) {
        Face& face = facePointer[i];

        pindex_buffer[i * 3 + 0] = face.a;
        pindex_buffer[i * 3 + 1] = face.b;
        pindex_buffer[i * 3 + 2] = face.c;
    }

    meshData->indexStream.nIndices = face_count * 3;
    meshData->indexStream.indexSize = face_count * 3 *sizeof(GLushort);

    GLint mode;
//    if (geometry.isStatic()) {
//        mode = GL_STATIC_DRAW;
//    } else {
        mode = GL_DYNAMIC_DRAW;
//    }

    if (meshData->indexStream.vboIB == 0) {
        glGenBuffers(1, &(meshData->indexStream.vboIB));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->indexStream.vboIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData->indexStream.indexSize, pindex_buffer, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    for (auto& unit : meshData->vertexStreams) {
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

void BufferObjectUtil::updateGeometryToHardware(Mesh &mesh)
{
    MeshDataPtr meshData = mesh.getMeshData();

    for (auto& unit : meshData->vertexStreams) {
        VertexBuffer& stream = unit.stream;

        glBindBuffer(GL_ARRAY_BUFFER, stream.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, stream.size, (GLvoid*)unit.vertices.getData());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

}
