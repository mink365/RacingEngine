#ifndef FBXPARSER_H
#define FBXPARSER_H

#include <streambuf>
#include <vector>
#include "Scene/Mesh.h"

namespace re {

class Node;
class ReadCommon;

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

class FBXCluster;
class ClusterCollection;
typedef std::shared_ptr<FBXCluster> FBXClusterPtr;
typedef std::shared_ptr<ClusterCollection> ClusterCollectionPtr;
struct FBXCluster {
    Long linkedMeshId;
    Long linkedBoneId;

    Int linkMode;

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
    void parseStream(std::istream *st);

    void parseData(void *data, long datalen);

    std::vector<SceneNodePtr> getNodes() const;

private:
    SceneNodePtr readNode(std::istream *st);
    void readMesh(std::istream *st, SceneNodePtr node);

    void readSkeleton(std::istream *st);
    BoneNodePtr readBoneNode(std::istream* st, AnimationPtr animation);
    KeyFrame readKeyFrame(std::istream* st);
    FBXClusterPtr readCluster(std::istream* st);

    void readNodeTransform(std::istream *st, NodePtr node);

    void readMaterial(std::istream *st, MeshPtr mesh);

    std::vector<SceneNodePtr> nodes;
    std::vector<SkeletonPtr> skeletons;
    std::vector<AnimationPtr> animations;
    std::vector<ClusterCollectionPtr> clusterColls;

    ReadCommon *reader;
};

} // namespace re

#endif // FBXPARSER_H
