#ifndef FBXPARSER_H
#define FBXPARSER_H

#include <streambuf>
#include <vector>
#include "Scene/Mesh.h"
#include "Scene/Transform.h"
#include "Animation/Skeleton.h"
#include "FileSystem/File.h"

using std::vector;
using std::istream;

namespace re {

class Node;
class ReadCommon;
class SkeletonController;

enum class FbxNodeAttributeType {
    GROUP = 1,
    SKELETON = 3,
    MESH = 4
};

class membuf : public std::basic_streambuf<char>
{
public:
  membuf(char* p, size_t n) {
    setg(p, p, p + n);
    setp(p, p + n);
  }
};

struct FBXCluster;
struct ClusterCollection;
typedef SharedPtr<FBXCluster> FBXClusterPtr;
typedef SharedPtr<ClusterCollection> ClusterCollectionPtr;
struct FBXCluster {
    Long linkedMeshId;
    Long linkedBoneId;

    LinkMode linkMode;

    vector<Int> linkIndices;
    vector<float> weightValues;

    Mat4 transformMatrix;
    Mat4 transformLinkMatrix;
};

struct ClusterCollection {
    Long meshId;

    vector<FBXClusterPtr> clusters;
};

class FbxParser
{
public:
    FbxParser();

    void parse(const string &path);
    void parse(FilePtr& file);
    void parseStream(std::istream *st);

    void parseData(void *data, Long datalen);

    const std::vector<NodePtr>& getNodes() const;
    NodePtr getNode(const string &name) const;
    NodePtr getSkinningNode(const string& name) const;

    ClusterCollectionPtr getClusterCollection(Long id) const;
    SkeletonPtr getSkeleton(Long id) const;

private:
    NodePtr readNode(std::istream *st);
    void readMesh(std::istream *st, NodePtr node);

    void readSkeleton(std::istream *st);
    BonePtr readBoneNode(std::istream* st, AnimationPtr animation);
    KeyFrame readKeyFrame(std::istream* st);
    FBXClusterPtr readCluster(std::istream* st);

    void readNodeTransform(std::istream *st, NodePtr node);

    PassPtr readMaterialPass(std::istream *st, MaterialPtr& material);

    void bindClusterData();

private:
    std::vector<NodePtr> nodes;
    std::vector<SkeletonPtr> skeletons;
    std::vector<AnimationPtr> animations;
    std::vector<ClusterCollectionPtr> clusterColls;

    ReadCommon *reader;

    std::string materialTextureKey;
};

} // namespace re

#endif // FBXPARSER_H
