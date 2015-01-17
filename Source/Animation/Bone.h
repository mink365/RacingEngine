#ifndef BONENODE_H
#define BONENODE_H

#include "platform.h"

#include <vector>

#include "Scene/Node.h"
#include "Scene/Component.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"

namespace re {

class AnimationTrack;
class Mat4;
class Bone;

typedef shared_ptr<Bone> BonePtr;

class Bone : public Component
{
    friend class Skeleton;
    friend class FbxParser;

public:
    Bone();

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
