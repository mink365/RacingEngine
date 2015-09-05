#ifndef SKELETON_H
#define SKELETON_H

#include <vector>
#include "platform.h"
#include "Base/ECS/Component.h"

namespace re {

class Mat4;
class Vec3;
class Bone;
class Skeleton;

typedef ComponentHandle<Bone> BonePtr;
typedef SharedPtr<Skeleton> SkeletonPtr;

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

    void setRootBone(BonePtr bone);
    BonePtr getRootBone() const;

    BonePtr getBone(Long id);

    void compute(std::vector<Mat4>& boneDeformations, std::vector<float>& boneWeights, const Mat4& meshGeometryMatrix, const Mat4& globalPositionMatrix);

private:
    void cacheBones();

    void computeBone(BonePtr bone, std::vector<Mat4>& boneDeformations, std::vector<float>& boneWeights, const Mat4& meshGeometryMatrix, const Mat4& globalPositionMatrix);

    void updateBoneVertex(std::vector<Vec3>& boneVertex);

private:
    LinkMode linkMode;

    BonePtr rootBone;
    std::vector<BonePtr> bones;
};

} // namespace re

#endif // SKELETON_H
