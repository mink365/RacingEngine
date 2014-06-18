#ifndef BONENODE_H
#define BONENODE_H

#include "platform.h"

#include <vector>

#include "Scene/SceneNode.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"

namespace re {

class AnimationTrack;
class Mat4;
class BoneNode;

typedef shared_ptr<BoneNode> BoneNodePtr;

class BoneNode : public Node
{
    friend class Skeleton;
    friend class FbxParser;

public:
    BoneNode();

    void setType(Int type);
    void setLinkMode(LinkMode type);

    Int getNumLinkedControlPoints();

    void setAnimationTrack(AnimationTrackPtr track);

    Mat4 getVertexTransformMatrix(const Mat4 &meshGeometryMatrix, const Mat4 &globalPositionMatrix);

private:
    void updateMatrix();

private:
    Int type;
    LinkMode linkMode;

    AnimationTrackPtr animationTrack;
    Mat4 transformMatrix;
    Mat4 transformLinkMatrix;

    vector<Int> linkIndices;
    vector<float> weightValues;
};

} // namespace re

#endif // BONENODE_H
