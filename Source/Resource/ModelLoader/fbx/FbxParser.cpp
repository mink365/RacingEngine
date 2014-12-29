#include "FbxParser.h"
#include "ModelLoader/ReadCommon.h"
#include "Math/Vector.h"
#include "Scene/Node.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/BoneNode.h"
#include "Animation/SkeletonController.h"

#include <iostream>
#include <vector>
#include "Texture/TextureManager.h"
#include "Material/MaterialManager.h"
#include "Util/ContainerUtil.h"
#include "Base/Buffer.h"
#include "FileSystem/FileSystem.h"
#include "Util/PrintUtil.h"

namespace re {

FbxParser::FbxParser()
{
    this->reader = new ReadCommon();
}

void FbxParser::parse(const string &path)
{
    FilePtr file = FileSystem::getInstance().getFile(path);
    ByteBufferPtr buf = file->read();

    this->parseData(buf->getData(), buf->getSize());
}

void FbxParser::parse(FilePtr &file)
{
    ByteBufferPtr buf = file->read();

    this->parseData(buf->getData(), buf->getSize());
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

    this->bindClusterData();
}

void FbxParser::parseData(void *data, long datalen)
{
    membuf mb((char*)data, datalen);
    std::istream stream(&mb);

    this->parseStream(&stream);
}

void FbxParser::readNodeTransform(std::istream *st, NodePtr node) {
    Vec3 transform = reader->ReadVec3(st);
    Vec3 rotation = reader->ReadVec3(st);
    Vec3 scale = reader->ReadVec3(st);

//    PrintVector(&transform);
//    PrintVector(&scale);
//    PrintVector(&rotation);

    rotation *= DEG_TO_RAD;

    node->localTranslation.set(transform);
    node->localRotation.fromAngles(rotation);
    node->localScaling.set(scale);

    node->markLocalTransformRefreshFlag();
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

void FbxParser::readMesh(std::istream *st, SceneNodePtr node) {
    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->init();
    node->setNodeAttribute(mesh);
    mesh->name = node->name;

    SkinnedMeshDataPtr meshData = std::make_shared<SkinnedMeshData>();
    mesh->setMeshData(meshData);

    StreamUnit unit;
    unit.format.push_back(VertexElement(VertexElementType::Position, AttributeFormat::FLOAT, 3));
    unit.format.push_back(VertexElement(VertexElementType::TextureCoord, AttributeFormat::FLOAT, 2));
    unit.format.push_back(VertexElement(VertexElementType::Normal, AttributeFormat::FLOAT, 3));
    unit.format.push_back(VertexElement(VertexElementType::Diffuse, AttributeFormat::FLOAT, 4));
    meshData->vertexStreams.push_back(unit);

    // controller points
    int len = reader->ReadInt(st);
    float points[len * 3];

    std::cout << " count: " << len << std::endl;

    st->read((char*)points, len * 3 * 4);
    for (int i = 0; i < len; ++i) {
        Vec3 xyz(points[i * 3 + 0], points[i * 3 + 1], points[i * 3 + 2]);

        meshData->controlPointsData.controlPoints.push_back(xyz);
    }

    PrintArray("points ", points, len * 3, 3);

    len = reader->ReadInt(st);
    uint vertexToControl[len];
    st->read((char*)vertexToControl, len * 4);

    PrintArray(" vertex to control ", vertexToControl, len, 1);

    // TODO: give the data to mesh
    VectorCopy(vertexToControl, 0, len, meshData->controlPointsData.vertexToControl);

    len = reader->ReadInt(st);
    short index[len];
    st->read((char*)index, len * 2);

    PrintArray("index ", index, len, 1);

    int num_of_indices = len;

    Buffer<uint>& indices = mesh->getMeshData()->indices;
    indices.allocate(num_of_indices);
    Face* facePointer = Map<Face>(indices);
    for (int i = 0; i < num_of_indices/3; ++i) {
        Face& face = facePointer[i];
        face.a = index[i*3];
        face.b = index[i*3 + 1];
        face.c = index[i*3 + 2];
    }
    std::cout << " num of indices " << len << std::endl;

    len = reader->ReadInt(st);
    int num_of_vertex = len;

    float vertex[len];
    if (len > 0) {
        st->read((char*)vertex, len * 4);

        PrintArray("vertex ", vertex, len, 3);
    }

    int colorCount = 0;
    float* colorArray = NULL;
    do
    {
        colorCount = reader->ReadInt(st);

        if (colorCount > 0) {
            colorArray = new float[len];

            st->read((char*)colorArray, colorCount * 4);

            PrintArray("color ", colorArray, colorCount, 4);
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

            PrintArray("UV ", uvArray, uvCount, 2);
        }
    } while (uvCount != -1);

    int normalCount = reader->ReadInt(st);
    float* normalArray = NULL;
    if (normalCount > 0) {
        normalArray = new float[normalCount];
        st->read((char*)normalArray, normalCount * 4);

        PrintArray("normal ", normalArray, normalCount, 3);
    }
    std::cout << " normal " << len << std::endl;

    // push data
    int vertexCount = num_of_vertex / 3;
    auto& vertices = mesh->getMeshData()->vertexStreams[0].vertices;
    vertices.allocate(vertexCount * sizeof(FbxVertex));
    auto vertexPointer = Map<FbxVertex>(vertices);
    for (int i = 0; i < vertexCount; ++i) {
        FbxVertex v;
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

        vertexPointer[i] = v;
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
    PassPtr pass = this->readMaterialPass(st);

    // bone and link
    len = reader->ReadInt(st);
    if (len > 0) {
        // have some bone, is a dynamic mesh

        // TODO: Dynamic
//        mesh->getGeometry()->staticGeometry = false;

        ClusterCollectionPtr clusterColl = std::make_shared<ClusterCollection>();
        clusterColl->meshId = node->id;

        for (int i = 0; i < len; ++i) {
            FBXClusterPtr cluster = this->readCluster(st);

            cluster->linkedMeshId = node->id;

            clusterColl->clusters.push_back(cluster);
        }

        this->clusterColls.push_back(clusterColl);
    }

    if (pass != nullptr) {
        MaterialPtr material = MaterialManager::getInstance().getMaterial(materialTextureKey);
        if (material != nullptr) {
            //TODO: if the pass is in material file ?
            auto m = material->clone();
            m->addPass(pass);

            mesh->setMaterial(m);
        } else {
            mesh->getMaterial()->clearPasses();
            mesh->getMaterial()->addPass(pass);
        }
    } else {
        // TODO: don't show the node with no material
        node->setVisible(false);
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

    vectorR *= DEG_TO_RAD;

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
        bone->setAnimationTrack(track);

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

    vectorR *= DEG_TO_RAD;

    KeyFrame frame(time, vectorT, vectorR.toQuat(), vectorS);

    return frame;
}

FBXClusterPtr FbxParser::readCluster(istream *st)
{
    FBXClusterPtr cluster = std::make_shared<FBXCluster>();

    cluster->linkedBoneId = reader->ReadLong(st);
    cluster->linkMode = (LinkMode)reader->ReadInt(st);

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

PassPtr FbxParser::readMaterialPass(std::istream *st) {
    PassPtr pass = nullptr;

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

            TextureUnitState::ptr unit = TextureUnitState::create();
            unit->setUVstate(offsetU, offsetV, scaleU, scaleV, 0);

            Texture::ptr tex = TextureManager::getInstance().getTexture(name);
            if (tex == nullptr) {
                assert(false);
            }
            unit->setTexture(tex);

            // TODO: fix the material name
            materialTextureKey = name;

            if (pass == nullptr) {
                pass = Pass::create();
            }
            pass->addTextureUnit(unit);

            std::cout << "id: " << id << " " << name << " offset " << offsetU << " " << offsetV << " scale " << scaleU << " " << scaleV << std::endl;
        }

    } while (id != -1);

    return pass;
}

void FbxParser::bindClusterData()
{
    for (auto coll : this->clusterColls) {
        Long boneId = coll->clusters.at(0)->linkedBoneId;
        SkeletonPtr skeleton = this->getSkeleton(boneId);
        assert(skeleton);

        for (auto cluster : coll->clusters) {
            auto bone = skeleton->getBone(cluster->linkedBoneId);

            bone->linkIndices = cluster->linkIndices;
            bone->weightValues = cluster->weightValues;

            bone->setLinkMode(cluster->linkMode);

            bone->transformMatrix = cluster->transformMatrix;
            bone->transformLinkMatrix = cluster->transformLinkMatrix;
        }
    }
}

std::vector<SceneNodePtr> FbxParser::getNodes() const
{
    return nodes;
}

SceneNodePtr FbxParser::getSceneNode(const string &name) const
{
    for (auto node : this->nodes) {
        if (node->getName() == name) {
            return node;
        }
    }

    return nullptr;
}

ClusterCollectionPtr FbxParser::getClusterCollection(Long id) const
{
    for (auto coll : this->clusterColls) {
        if (coll->meshId == id) {
            return coll;
        }
    }

    return nullptr;
}

SkeletonPtr FbxParser::getSkeleton(Long id) const
{
    for (auto skeleton : this->skeletons) {
        if (skeleton->getBone(id) != nullptr) {
            return skeleton;
        }
    }

    return nullptr;
}

SkeletonControllerPtr FbxParser::getSkeletonController(const string &name) const
{
    auto node = this->getSceneNode(name);
    assert(node != nullptr);

    auto coll = this->getClusterCollection(node->getId());

    Long boneId = coll->clusters.at(0)->linkedBoneId;
    auto skeleton = this->getSkeleton(boneId);

    auto animation = this->animations.at(0);

    SkeletonControllerPtr controller = std::make_shared<SkeletonController>(node, skeleton, animation);

    return controller;
}

}
