#ifndef SKELETONCONTROLLER_H
#define SKELETONCONTROLLER_H

#include "platform.h"
#include <vector>
#include "Math/Matrix.h"

namespace re {

class SkeletonController;
typedef shared_ptr<SkeletonController> SkeletonControllerPtr;

class SkeletonController
{
public:
    SkeletonController(SceneNodePtr mesh, SkeletonPtr skeleton, AnimationPtr animation);

    SkeletonPtr getSkeleton();
    AnimationPtr getAnimation();
    SceneNodePtr getMeshNode();
    MeshPtr getMesh();

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
    SkinnedMeshDataPtr meshData;

    std::vector<Mat4> boneDeformations;
    std::vector<float> boneWeights;
    size_t controlPointsCount;

private:
    Mat4 emptyMatrix;

    bool isPlaying;
};
}


#endif // SKELETONCONTROLLER_H
