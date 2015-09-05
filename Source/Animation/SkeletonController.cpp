#include "SkeletonController.h"

#include "Skeleton.h"
#include "Animation.h"
#include "AnimationTrack.h"
#include "Bone.h"
#include "Scene/Node.h"
#include "Render/Mesh.h"
#include "Scene/RenderElement.h"
#include "Scene/Transform.h"
#include "Platform/GameHub.h"

SkeletonController::SkeletonController()
{
    float* mat = this->emptyMatrix.toFloatPtr();
    std::fill(mat, mat+16, 0);

    this->isPlaying = false;
}

re::SkeletonController::SkeletonController(SkeletonPtr skeleton, AnimationPtr animation)
    : SkeletonController()
{
    this->skeleton = skeleton;
    this->animation = animation;
}

void re::SkeletonController::init()
{
    this->mesh = this->getComponent<RenderElement>()->getMesh();
    this->meshData = std::dynamic_pointer_cast<SkinnedMeshData>(this->mesh->getMeshData());
}

re::SkeletonPtr re::SkeletonController::getSkeleton()
{
    return this->skeleton;
}

re::AnimationPtr re::SkeletonController::getAnimation()
{
    return this->animation;
}

void re::SkeletonController::play()
{
    this->isPlaying = true;

    this->setInitTime(GameHub::instance().GetGameTime().GetMilliSecond());
}

void re::SkeletonController::stop()
{
    this->isPlaying = false;
}

void re::SkeletonController::reset()
{
    this->setDefaultFrame(0);
}

void re::SkeletonController::update()
{
    if (!isPlaying) {
        return;
    }

    auto time = GameHub::instance().GetGameTime();
    this->setCurrTime(time.GetMilliSecond());

    this->computeLinearDeformation();
}

void re::SkeletonController::setDefaultFrame(int frame)
{
    this->animation->setIsUseAnimationStack(false);

    this->animation->setAnimationLoop(false);
    this->animation->setAnimationPower(1.0f);

    this->setInitTime(0);
    Long time = this->animation->getCurrAnimationTrack()->getKeyFrame(frame)->getTime();
    this->setCurrTime(time);

    this->computeLinearDeformation();
}

void re::SkeletonController::setInitTime(Long time)
{
    this->animation->setBeginTime(time);
}

void re::SkeletonController::setCurrTime(Long time)
{
    this->animation->setCurrTime(time);
}

void re::SkeletonController::computeLinearDeformation()
{
    this->controlPointsCount = meshData->controlPointsData.controlPoints.size();

    if (this->controlPointsCount >= this->boneWeights.size()) {
        this->initMatrixVector();
    } else {
        this->resetMatrixVector();
    }

    Mat4 identity;
    this->skeleton->compute(this->boneDeformations, this->boneWeights, identity,
                            this->getEntity()->getTransform()->getLocalMatrix());

    for (size_t i = 0; i < this->controlPointsCount; ++i) {
        Vec3& xyz = meshData->controlPointsData.controlPoints[i];

        Vec3 srcVertex(xyz);
        Vec3 destVertex(xyz);

        float weight = boneWeights[i];
        if (weight > 0) {
            Mat4& m = boneDeformations[i];

            destVertex = m * srcVertex;

            if (this->skeleton->linkMode == LinkMode::Normalize) {
                destVertex *= (1 / weight);
            } else if (this->skeleton->linkMode == LinkMode::Total) {
                destVertex *= (1.0 - weight);

                destVertex += srcVertex;
            }
        }

        if (meshData->controlPointsData.controlToVertex.size() != 0) {
            std::vector<uint>& vertexList = meshData->controlPointsData.controlToVertex.at(i);

            for (auto& index : vertexList) {
                meshData->setVertex(index, destVertex);
            }
        } else {
            for (size_t j = 0; j < meshData->controlPointsData.vertexToControl.size(); ++j) {
                Int controlPointIndex = meshData->controlPointsData.vertexToControl[j];

                if (controlPointIndex == i) {
                    meshData->setVertex(j, destVertex);
                }
            }
        }
    }
}

void re::SkeletonController::resetMatrixVector()
{
    this->boneDeformations.assign(this->controlPointsCount, emptyMatrix);
    this->boneWeights.assign(this->controlPointsCount, 0);
}

void re::SkeletonController::initMatrixVector()
{
    this->meshData->controlPointsData.cacheVertex();

    this->boneDeformations.resize(this->controlPointsCount, emptyMatrix);
    this->boneWeights.resize(this->controlPointsCount);

    this->resetMatrixVector();
}
