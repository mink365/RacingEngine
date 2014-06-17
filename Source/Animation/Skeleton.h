#ifndef SKELETON_H
#define SKELETON_H

#include <vector>
#include "platform.h"

namespace re {

class Mat4;
class Vec3;
class Skeleton;

typedef shared_ptr<Skeleton> SkeletonPtr;

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
    Normalize = 0,
    Additive = 1,
    Total = 2,
};

/**
 * @brief The Skeleton class
 */
class Skeleton
{
    friend class SkeletonController;

public:
    Skeleton();

    void setRootBone(BoneNodePtr bone);
    BoneNodePtr getRootBone() const;

    BoneNodePtr getBone(Long id);

    void compute(vector<Mat4>& boneDeformations, vector<float>& boneWeights, const Mat4& meshGeometryMatrix, const Mat4& globalPositionMatrix);

private:
    void cacheBones(BoneNodePtr bone);

    void computeBone(BoneNodePtr bone, vector<Mat4>& boneDeformations, vector<float>& boneWeights, const Mat4& meshGeometryMatrix, const Mat4& globalPositionMatrix);

    void updateBoneVertex(BoneNodePtr bone, vector<Vec3>& boneVertex, Int Index);

private:
    LinkMode linkMode;

    BoneNodePtr rootBone;
    std::vector<BoneNodePtr> bones;
};

} // namespace re

#endif // SKELETON_H
