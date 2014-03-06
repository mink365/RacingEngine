#ifndef SKELETON_H
#define SKELETON_H

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
};

#endif // SKELETON_H
