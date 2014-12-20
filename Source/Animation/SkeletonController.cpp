#include "SkeletonController.h"

#include "Skeleton.h"
#include "Animation.h"
#include "BoneNode.h"
#include "Scene/SceneNode.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Scene/Mesh.h"

re::SkeletonController::SkeletonController(SceneNodePtr mesh, SkeletonPtr skeleton, AnimationPtr animation)
{
    float* mat = this->emptyMatrix.toFloatPtr();
    std::fill(mat, mat+16, 0);

    this->sceneNode = mesh;
    this->skeleton = skeleton;
    this->animation = animation;

    this->mesh = dynamic_pointer_cast<Mesh>(this->sceneNode->getNodeAttribute());
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

re::SceneNodePtr re::SkeletonController::getMeshNode()
{
    return this->sceneNode;
}

re::MeshPtr re::SkeletonController::getMesh()
{
    return this->mesh;
}

void re::SkeletonController::play()
{

}

void re::SkeletonController::stop()
{

}

void re::SkeletonController::reset()
{

}

Long local_time = 0;
void re::SkeletonController::update()
{
    local_time += 20;

    this->animation->setCurrTime(local_time);

    this->computeLinearDeformation();
}

void re::SkeletonController::setDefaultFrame(int frame)
{
    this->animation->setIsUseAnimationStack(false);

    this->animation->setAnimationLoop(false);
    this->animation->setAnimationPower(1.0f);

    Long time = this->animation->getCurrAnimationTrack()->getKeyFrame(frame)->getTime();

    this->animation->setCurrTime(time);

    this->computeLinearDeformation();
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
    this->skeleton->compute(this->boneDeformations, this->boneWeights, identity, this->sceneNode->getLocalMatrix());

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
            vector<uint>& vertexList = meshData->controlPointsData.controlToVertex.at(i);

            for (auto& index : vertexList) {
//                Vertex& vertex = geometry->getVertices().at(index);
//                vertex.xyz = destVertex;
                meshData->setVertex(index, destVertex);
            }
        } else {
            for (size_t j = 0; j < meshData->controlPointsData.vertexToControl.size(); ++j) {
                Int controlPointIndex = meshData->controlPointsData.vertexToControl[j];

                if (controlPointIndex == i) {
//                    Vertex& vertex = geometry->getVertices().at(j);
//                    vertex.xyz = destVertex;
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
