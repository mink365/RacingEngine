#ifndef BONENODE_H
#define BONENODE_H

#include "platform.h"

#include <vector>

#include "scene/Node.h"
#include "animation/Animation.h"
#include "animation/Skeleton.h"

namespace re {

class AnimationTrack;
class Mat4;
class BoneNode;

typedef shared_ptr<BoneNode> BoneNodePtr;

class BoneNode : public Node
{
public:
    BoneNode();

    void setType(Int type);
    void setLinkMode(LinkMode type);

    Int getNumLinkedControlPoints();

    const Mat4& getLocalMatrix() const override;
    const Mat4& getGlobalMatrix() const;

    Mat4 getVertexTransformMatrix(const Mat4 &meshGeometryMatrix, const Mat4 &globalPositionMatrix);

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
