#include "FbxParser.h"
#include "Tools/ModelLoader/ReadCommon.h"
#include "Math/Vector.h"
#include "Scene/Node.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/BoneNode.h"

#include <iostream>
#include <istream>
#include <vector>
#include <sstream>
#include "Texture/TextureManager.h"
#include "Util/ContainerUtil.h"

namespace re {

FbxParser::FbxParser()
{
    this->reader = new ReadCommon();
}

void FbxParser::parse(const string &path)
{
    std::ifstream filestr;
    filestr.open(path.c_str(), ios::binary);

    this->parseStream(&filestr);

    filestr.close();
}

void FbxParser::parseStream(std::istream *st) {
    this->nodes.clear();
    this->skeletons.clear();
    this->animations.clear();
    this->clusterColls.clear();

    int type = reader->ReadInt(st);
    int mode_len = reader->ReadInt(st);
    int index = reader->ReadInt(st);
    int count = reader->ReadInt(st);

    std::cout << "type: " << type << " len: " << mode_len << " index: " << index << " count: " << count << std::endl;

    for (int i = 0; i < count; ++i) {
        auto node = readNode(st);

        if (node != nullptr) {
            this->nodes.push_back(node);
        }
    }
}

void FbxParser::parseData(void *data, long datalen)
{
    membuf mb((char*)data, datalen);
    std::istream stream(&mb);

    this->parseStream(&stream);
}

SceneNodePtr FbxParser::readNode(std::istream *st) {
    FbxNodeAttributeType attType = (FbxNodeAttributeType)reader->ReadInt(st);

    switch (attType) {
    case FbxNodeAttributeType::MESH:
    case FbxNodeAttributeType::GROUP:
    {
        SceneNodePtr node = std::make_shared<SceneNode>();

        long id = reader->ReadLong(st);
        std::string name = reader->ReadString(st);

        node->id = id;
        node->name = name;

        std::cout << " node: " << name << " id: " << id << " att: " << (int)attType << std::endl;

        this->readNodeTransform(st, node);

        if (attType == FbxNodeAttributeType::MESH) {
            std::cout << " read mesh" << std::endl;

            this->readMesh(st, node);
        }

        int child_count = reader->ReadInt(st);
        for (int i = 0; i < child_count; ++i) {
            auto child = this->readNode(st);

            node->addChild(child);
        }

        return node;
    }
        break;
    case FbxNodeAttributeType::SKELETON:
        this->readSkeleton(st);
        break;
    default:
        // 摄像机和灯光信息
        std::cout << " not supported att type: " << (int)attType << std::endl;
        break;
    }

    return nullptr;
}

void printV(Vec3 *v) {
    std::cout << "V: " << v->x << " "<< v->y << " "<< v->z << std::endl;
}

void FbxParser::readNodeTransform(std::istream *st, NodePtr node) {
    Vec3 transform = reader->ReadVec3(st);
    Vec3 rotation = reader->ReadVec3(st);
    Vec3 scale = reader->ReadVec3(st);

    printV(&transform);
    printV(&scale);
    printV(&rotation);

    node->localTranslation.set(transform);
    node->localRotation.fromAngles(rotation);
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

void FbxParser::readMesh(std::istream *st, SceneNodePtr node) {
    MeshPtr mesh = std::make_shared<Mesh>();
    node->setNodeAttribute(mesh);
    mesh->name = node->name;

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

        mesh->getGeometry().addFace(f);
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

        mesh->getGeometry().addVertex(v);
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
    this->readMaterial(st, mesh);

    // bone and link
    len = reader->ReadInt(st);
    if (len > 0) {
        // have some bone

        ClusterCollectionPtr clusterColl = std::make_shared<ClusterCollection>();
        clusterColl->meshId = node->id;

        for (int i = 0; i < len; ++i) {
            FBXClusterPtr cluster = this->readCluster(st);

            cluster->linkedMeshId = node->id;

            clusterColl->clusters.push_back(cluster);
        }

        this->clusterColls.push_back(clusterColl);
    }

    std::cout << "mesh read done. " << std::endl;
}

void FbxParser::readSkeleton(istream *st)
{
    SkeletonPtr skeleton = std::make_shared<Skeleton>();
    AnimationPtr animation = std::make_shared<Animation>();

    BoneNodePtr bone = this->readBoneNode(st, animation);

    skeleton->setRootBone(bone);

    this->skeletons.push_back(skeleton);
    this->animations.push_back(animation);
}

BoneNodePtr FbxParser::readBoneNode(istream *st, AnimationPtr animation)
{
    BoneNodePtr bone = std::make_shared<BoneNode>();

    Long id = reader->ReadLong(st);
    string name = reader->ReadString(st);
    Int type = reader->ReadInt(st);

    bone->id = id;
    bone->name = name;
    bone->type = type;

    Vec3 vectorT = reader->ReadVec3(st);
    Vec3 vectorR = reader->ReadVec3(st);
    Vec3 vectorS = reader->ReadVec3(st);

    bone->setLocalTranslation(vectorT);
    bone->setLocalRotation(vectorR.toQuat());
    bone->setLocalScaling(vectorS);

    Int keyCount = reader->ReadInt(st);
    if (keyCount > 0) {
        AnimationTrackPtr track = std::make_shared<AnimationTrack>();

        for (Int i = 0; i < keyCount; ++i) {
            KeyFrame frame = this->readKeyFrame(st);

            track->addKeyFrame(frame);
        }

        track->updateTrackInfo();
        track->boneNode = bone;

        animation->addAnimationTrack(track);
    }

    Int childCount = reader->ReadInt(st);
    for (int i = 0; i < childCount; ++i) {
        BoneNodePtr child = this->readBoneNode(st, animation);

        bone->addChild(child);
    }

    return bone;
}

KeyFrame FbxParser::readKeyFrame(istream *st)
{
    Long time = reader->ReadLong(st);

    Vec3 vectorT = reader->ReadVec3(st);
    Vec3 vectorR = reader->ReadVec3(st);
    Vec3 vectorS = reader->ReadVec3(st);

    KeyFrame frame(time, vectorT, vectorR.toQuat(), vectorS);

    return frame;
}

FBXClusterPtr FbxParser::readCluster(istream *st)
{
    FBXClusterPtr cluster = std::make_shared<FBXCluster>();

    cluster->linkedBoneId = reader->ReadLong(st);
    cluster->linkMode = reader->ReadInt(st);

    Int len = reader->ReadInt(st);

    Int linkIndices[len];
    float weightValues[len];

    st->read((char*)linkIndices, len * 4);
    st->read((char*)weightValues, len * 4);

    VectorCopy(linkIndices, 0, len, cluster->linkIndices);
    VectorCopy(weightValues, 0, len, cluster->weightValues);

    cluster->transformMatrix = reader->ReadMat4(st);
    cluster->transformLinkMatrix = reader->ReadMat4(st);

    return cluster;
}

void FbxParser::readMaterial(std::istream *st, MeshPtr mesh) {
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

            TextureUnitState &unit = mesh->getMaterial().getTexture();
            unit.setUVstate(offsetU, offsetV, scaleU, scaleV, 0);

            Texture &tex = TextureManager::getInstance().getTexture(name);
            unit.addTextureFrame(&tex);

            std::cout << "id: " << id << " " << name << " offset " << offsetU << " " << offsetV << " scale " << scaleU << " " << scaleV << std::endl;
        }

    } while (id != -1);
}

std::vector<SceneNodePtr> FbxParser::getNodes() const
{
    return nodes;
}

}
