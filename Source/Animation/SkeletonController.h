#ifndef SKELETONCONTROLLER_H
#define SKELETONCONTROLLER_H

#include "platform.h"
#include <vector>
#include "Math/Matrix.h"
#include "Scene/Component.h"

namespace re {

class SkeletonController;
typedef shared_ptr<SkeletonController> SkeletonControllerPtr;

class SkeletonController : public Component
{
public:
    SkeletonController();
    void init();

    SkeletonPtr getSkeleton();
    AnimationPtr getAnimation();
    NodePtr getMeshNode();
    MeshPtr getMesh();

    void play();
    void stop();
    void reset();

    void update();

    void setDefaultFrame(int frame);
    void setInitTime(Long time);
    void setCurrTime(Long time);

private:
    void computeLinearDeformation();

    void initMatrixVector();
    void resetMatrixVector();

private:
    SkeletonPtr skeleton;
    AnimationPtr animation;
    NodePtr sceneNode;
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
