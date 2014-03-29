#ifndef SKELETON_H
#define SKELETON_H

#include <vector>
#include "platform.h"

namespace re {

class Mat4;

/**
 * @brief The SkinningType enum
 *
 * The mode of skeleton act on mesh
 */
enum class SkinningType {
    Rigid,
    Linear,
    DualQuaternion,
    Blend,
};

/**
 * @brief The LinkMode enum
 *
 * The relationship mode of controlPoint and weight, or how the
 * weight act on controlPoint
 */
enum class LinkMode {
    Normalize,
    Additive,
    Total,
};

/**
 * @brief The Skeleton class
 */
class Skeleton
{
public:
    Skeleton();

    void setRootBone(BoneNodePtr bone);
    BoneNodePtr getRootBone();

    void compute(vector<Mat4>& boneDeformations, vector<float>& boneWeights, const Mat4& meshGeometryMatrix, const Mat4& globalPositionMatrix);

private:
    void cacheBones(BoneNodePtr bone);

    void computeBone(BoneNodePtr bone, vector<Mat4>& boneDeformations, vector<float>& boneWeights, const Mat4& meshGeometryMatrix, const Mat4& globalPositionMatrix);

    void updateBoneVertex(BoneNodePtr bone, vector<float>& boneVertex, Int Index);

private:
    LinkMode linkMode;

    BoneNodePtr rootBone;
    std::vector<BoneNodePtr> bones;
    std::vector<AnimationPtr> animations;
};

} // namespace re

#endif // SKELETON_H
