#include "Skeleton.h"

#include "math/Matrix.h"
#include "BoneNode.h"
#include "scene/Node.h"

namespace re {

void matrixScale(Mat4& matrix, float value) {
    for (int i = 0; i < 16; ++i) {
        matrix[i] *= value;
    }
}

void matrixAdd(Mat4& destMatrix, float srcMatrix) {
    for (int i = 0; i< 16; ++i) {
        destMatrix[i] += srcMatrix[i];
    }
}

Skeleton::Skeleton()
{
}

void Skeleton::cacheBones(BoneNodePtr bone)
{
    this->bones.push_back(bone);

    for (auto childBone : bone->getChildren()) {
        this->cacheBones(childBone);
    }
}

void Skeleton::compute(vector<Mat4> &boneDeformations, vector<float> &boneWeights, const Mat4 &meshGeometryMatrix, const Mat4 &globalPositionMatrix)
{
    BoneNodePtr rootBone = this->getRootBone();

    this->computeBone(rootBone, boneDeformations, boneWeights, meshGeometryMatrix, globalPositionMatrix);
}

void Skeleton::computeBone(BoneNodePtr bone, vector<Mat4> &boneDeformations, vector<float> &boneWeights, const Mat4 &meshGeometryMatrix, const Mat4 &globalPositionMatrix)
{
    Mat4 vertexTransformMatrix = bone->getVertexTransformMatrix(meshGeometryMatrix, globalPositionMatrix);

    for (Int i = 0; i < bone->getNumLinkedControlPoints(); ++i) {
        Int index = bone->linkIndices[i];
        float weight = bone->weightValues[i];

        if (weight <= 0) {
            continue;
        }

        Mat4 influence = vertexTransformMatrix;
        matrixScale(influence, weight);

        if (linkMode == LinkMode::Additive) {

        } else {
            matrixAdd(boneDeformations[i], influence);
            boneWeights[i] += weight;
        }

        for (auto childBone : bone->getChildren()) {
            this->computeBone(childBone, boneDeformations, boneWeights, meshGeometryMatrix, globalPositionMatrix);
        }
    }
}

void Skeleton::updateBoneVertex(BoneNodePtr bone, vector<float> &boneVertex, Int Index)
{
    Vec3 v = bone->getWorldMatrix().getTranslation();

    // TODO: ....

    for (auto childBone : bone->getChildren()) {
        Index++;

        this->updateBoneVertex(childBone, boneVertex, Index);
    }
}

}
