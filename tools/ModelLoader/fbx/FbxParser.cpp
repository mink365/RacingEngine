#include "FbxParser.h"
#include "tools/ModelLoader/ReadCommon.h"
#include "math/Vector.h"
#include "scene/Node.h"

#include <iostream>
#include <istream>
#include <vector>
#include <sstream>
#include <texture/TextureManager.h>

FbxParser::FbxParser()
{
    this->reader = new ReadCommon();
}

void FbxParser::parseStream(std::istream *st) {
    this->nodes.clear();

    int type = reader->ReadInt(st);
    int mode_len = reader->ReadInt(st);
    int index = reader->ReadInt(st);
    int count = reader->ReadInt(st);

    std::cout << "type: " << type << " len: " << mode_len << " index: " << index << " count: " << count << std::endl;

    for (int i = 0; i < count; ++i) {
        auto node = readNode(st);

        this->nodes.push_back(node);
    }
}

void FbxParser::parseData(void *data, long datalen)
{
    membuf mb((char*)data, datalen);
    std::istream stream(&mb);

    this->parseStream(&stream);
}

NodePtr FbxParser::readNode(std::istream *st) {
    NodePtr node;

    int att_type = reader->ReadInt(st);

    if (att_type == NODE_MESH) {
        node = dynamic_pointer_cast<Node>(std::make_shared<Mesh>());

        long id = reader->ReadLong(st);
        std::string name = reader->ReadString(st);

        node->id = id;
        node->name = name;

        std::cout << " node: " << name << " id: " << id << " att: " << att_type << std::endl;

        this->readNodeTransform(st, node);

        if (att_type == NODE_MESH) {
            std::cout << " read mesh" << std::endl;

            this->readMesh(st, dynamic_pointer_cast<Mesh>(node));
        }

        int child_count = reader->ReadInt(st);
        for (int i = 0; i < child_count; ++i) {
            auto child = this->readNode(st);

            node->addChild(child);
        }
    } else if (att_type == NODE_GROUP) {
        node = dynamic_pointer_cast<Node>(std::make_shared<SceneNode>());

        long id = reader->ReadLong(st);
        std::string name = reader->ReadString(st);

        node->id = id;
        node->name = name;

        std::cout << " node: " << name << " id: " << id << " att: " << att_type << std::endl;

        this->readNodeTransform(st, node);

        int child_count = reader->ReadInt(st);
        for (int i = 0; i < child_count; ++i) {
            auto child = this->readNode(st);

            node->addChild(child);
        }
    } else if (att_type == NODE_SKELETON) {
        std::cout << " not supported att type: " << att_type << std::endl;
    } else {
        // 摄像机和灯光信息
        std::cout << " not supported att type: " << att_type << std::endl;
    }

    return node;
}

void printV(reVec3 *v) {
    std::cout << "V: " << v->x << " "<< v->y << " "<< v->z << std::endl;
}

void FbxParser::readNodeTransform(std::istream *st, NodePtr node) {
    reVec3 transform = reader->ReadVec3(st);
    reVec3 rotation = reader->ReadVec3(st);
    reVec3 scale = reader->ReadVec3(st);

    printV(&transform);
    printV(&scale);
    printV(&rotation);

    node->localTranslation.set(transform);
//    node->localRotation.set(rotation);
    node->localScaling.set(scale);
}

void printFloatArray(std::string head, float *v, int count, int split) {

    std::ostringstream oss;

    oss << head << "Array: {" << "\n";
    for (int i = 0; i < count / split; ++i) {
        oss << "V[";
        for (int j = 0; j < split; ++j) {
            oss << v[i * split + j] << ", ";
        }

        oss << "]" << "\n";
    }

    std::cout << oss.str() << std::endl;
}

void printShortArray(std::string head, short *v, int count, int split) {

    std::ostringstream oss;

    oss << head << "Array: {" << "\n";
    for (int i = 0; i < count / split; ++i) {
        oss << "V[";
        for (int j = 0; j < split; ++j) {
            oss << v[i * split + j] << ", ";
        }

        oss << "]" << "\n";
    }

    std::cout << oss.str() << std::endl;
}

void printIntArray(std::string head, int *v, int count, int split) {

    std::ostringstream oss;

    oss << head << "Array: {" << "\n";
    for (int i = 0; i < count / split; ++i) {
        oss << "V[";
        for (int j = 0; j < split; ++j) {
            oss << v[i * split + j] << ", ";
        }

        oss << "]" << "\n";
    }

    std::cout << oss.str() << std::endl;
}

void FbxParser::readMesh(std::istream *st, MeshPtr node) {
    // controller points
    int len = reader->ReadInt(st);
    float points[len * 3];

    std::cout << " count: " << len << std::endl;

    st->read((char*)points, len * 3 * 4);

    printFloatArray("points ", points, len * 3, 3);

    len = reader->ReadInt(st);
    int vertexToControl[len];
    st->read((char*)vertexToControl, len * 4);

    printIntArray(" vertex to control ", vertexToControl, len, 1);

    len = reader->ReadInt(st);
    short index[len];
    st->read((char*)index, len * 2);

    printShortArray("index ", index, len, 1);

    int num_of_indices = len;

    // push face data
    for (int i = 0; i < num_of_indices; i += 3) {
        Face f(index[i], index[i + 1], index[i + 2]);

        node->getGeometry().addFace(f);
    }

    std::cout << " num of indices " << len << std::endl;

    len = reader->ReadInt(st);
    int num_of_vertex = len;

    float vertex[len];
    if (len > 0) {
        st->read((char*)vertex, len * 4);

        printFloatArray("vertex ", vertex, len, 3);
    }

    int colorCount = 0;
    float* colorArray = NULL;
    do
    {
        colorCount = reader->ReadInt(st);

        if (colorCount > 0) {
            colorArray = new float[len];

            st->read((char*)colorArray, colorCount * 4);

            printFloatArray("color ", colorArray, colorCount, 4);
        }
    } while (colorCount != -1);

    int uvCount = 0;
    float* uvArray = NULL;
    do
    {
        uvCount = reader->ReadInt(st);

        if (uvCount > 0) {
            uvArray = new float[uvCount];
            st->read((char*)uvArray, uvCount * 4);

            printFloatArray("UV ", uvArray, uvCount, 2);
        }
    } while (uvCount != -1);

    int normalCount = reader->ReadInt(st);
    float* normalArray = NULL;
    if (normalCount > 0) {
        normalArray = new float[normalCount];
        st->read((char*)normalArray, normalCount * 4);

        printFloatArray("normal ", normalArray, normalCount, 3);
    }
    std::cout << " normal " << len << std::endl;

    // push data
    int vertexCount = num_of_vertex / 3;
    for (int i = 0; i < vertexCount; ++i) {
        Vertex v;
        v.xyz.set(vertex[i * 3], vertex[i * 3 + 1], vertex[i * 3 + 2]);

        if (colorArray != NULL) {
            v.color.set(colorArray[i * 4], colorArray[i * 4 + 1], colorArray[i * 4 + 2], colorArray[i * 4 + 3]);
        }

        if (uvArray != NULL) {
            v.uv.set(uvArray[i * 2], uvArray[i * 2 + 1]);
        }

        if (normalArray != NULL) {
            v.normal.set(normalArray[i * 3], normalArray[i * 3 + 1], normalArray[i * 3 + 2]);
        }

        node->getGeometry().addVertex(v);
    }

    if (colorArray != NULL) {
        delete [] colorArray;
    }

    if (uvArray != NULL) {
        delete [] uvArray;
    }

    if (normalArray != NULL) {
        delete [] normalArray;
    }

    // material and pass
    this->readMaterial(st, node);

    // bone and link
    len = reader->ReadInt(st);
    if (len > 0) {
        // have some bone
    }

    std::cout << "mesh read done. " << std::endl;
}

void FbxParser::readMaterial(std::istream *st, MeshPtr node) {
    int id = -1;

    do {
        id = reader->ReadInt(st);

        if (id != -1) {
            std::string name = reader->ReadString(st);

            std::cout << " name: " << name << std::endl;

            float offsetU = reader->ReadFloat(st);
            float offsetV = reader->ReadFloat(st);
            float scaleU = reader->ReadFloat(st);
            float scaleV = reader->ReadFloat(st);

            TextureUnitState &unit = node->getMaterial().getTexture();
            unit.setUVstate(offsetU, offsetV, scaleU, scaleV, 0);

            Texture &tex = TextureManager::getInstance().getTexture(name);
            unit.addTextureFrame(&tex);

            std::cout << "id: " << id << " " << name << " offset " << offsetU << " " << offsetV << " scale " << scaleU << " " << scaleV << std::endl;
        }

    } while (id != -1);
}

std::vector<NodePtr> FbxParser::getNodes() const
{
    return nodes;
}
