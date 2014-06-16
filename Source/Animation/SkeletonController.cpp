#include "SkeletonController.h"

#include "Skeleton.h"
#include "Animation.h"
#include "BoneNode.h"
#include "Scene/SceneNode.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Scene/Mesh.h"

re::SkeletonController::SkeletonController()
{
    float* mat = this->emptyMatrix.toFloatPtr();
    std::fill(mat, mat+16, 0);
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

}

void re::SkeletonController::computeLinearDeformation()
{
    if (this->controlPointsCount >= this->boneWeights.size()) {
        this->initMatrixVector();
    } else {
        this->resetMatrixVector();
    }

    Mat4 identity;
    this->skeleton->compute(this->boneDeformations, this->boneWeights, identity, this->sceneNode->getLocalMatrix());

    for (int i = 0; i < this->controlPointsCount; ++i) {

        Vec3 srcVertex;
        Vec3 destVertex;

        float weight = boneWeights[i];
        if (weight > 0) {
            Mat4& m = boneDeformations[i];

            // TODO: !!!
//            destVertex = m * srcVertex;

            if (this->skeleton->linkMode == LinkMode::Normalize) {
                destVertex *= (1 / weight);
            } else if (this->skeleton->linkMode == LinkMode::Total) {
                destVertex *= (1.0 - weight);

                destVertex += srcVertex;
            }
        }

        Geometry& geometry = this->mesh->getGeometry();

        if (geometry.controlToVertex.size() != 0) {
            vector<int>& vertexList = geometry.controlToVertex.at(i);

            for (auto& index : vertexList) {
                Vertex vertex = geometry.getVertices().at(index);

                vertex.xyz = destVertex;
            }
        } else {
            for (int j = 0; j < geometry.vertexToControl.size(); ++j) {
                Int controlPointIndex = geometry.vertexToControl[j];

                if (controlPointIndex == i) {
                    Vertex vertex = geometry.getVertices().at(j);
                    vertex.xyz = destVertex;
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
    this->boneDeformations.resize(this->controlPointsCount, emptyMatrix);
    this->boneWeights.resize(this->controlPointsCount);

    this->resetMatrixVector();
}
