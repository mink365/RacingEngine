#ifndef SKELETONCONTROLLER_H
#define SKELETONCONTROLLER_H

#include "platform.h"
#include <vector>
#include "Math/Matrix.h"

namespace re {
class SkeletonController
{
public:
    SkeletonController();

    void play();
    void stop();
    void reset();

    void update();

    void setDefaultFrame(int frame);

private:
    void computeLinearDeformation();

    void initMatrixVector();
    void resetMatrixVector();

private:
    SkeletonPtr skeleton;
    AnimationPtr animation;
    SceneNodePtr sceneNode;
    MeshPtr mesh;

    std::vector<Mat4> boneDeformations;
    std::vector<float> boneWeights;
    Int controlPointsCount;

private:
    Mat4 emptyMatrix;
};
}


#endif // SKELETONCONTROLLER_H
