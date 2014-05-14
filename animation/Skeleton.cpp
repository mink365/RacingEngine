﻿#include "Skeleton.h"

#include "math/Matrix.h"
#include "math/Vector.h"
#include "BoneNode.h"
#include "scene/Node.h"

namespace re {

void matrixScale(Mat4& matrix, float value) {
    for (int i = 0; i< 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] *= value;
        }
    }
}

void matrixAdd(Mat4& destMatrix, Mat4& srcMatrix) {
    for (int i = 0; i< 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            destMatrix[i][j] += srcMatrix[i][j];
        }
    }
}

Skeleton::Skeleton()
    : linkMode(LinkMode::Normalize)
{
}

void Skeleton::setRootBone(BoneNodePtr bone)
{
    this->rootBone = bone;

    this->linkMode = bone->linkMode;
}

BoneNodePtr Skeleton::getRootBone()
{
    return this->rootBone;
}

void Skeleton::cacheBones(BoneNodePtr bone)
{
    this->bones.push_back(bone);

    for (auto childBone : bone->getChildren()) {
        this->cacheBones(dynamic_pointer_cast<BoneNode>(childBone));
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

        /*
         * influence在matrixScale中被改变，对于每一个控点的计算，influence需要被设置为由骨骼计算来的顶点变换矩阵。根本原因是，
         * 虽然骨骼link的控点都有共同的vertexTransformMatrix但是，每个控点有不同的weight（权重）
         */
        Mat4 influence = vertexTransformMatrix;
        matrixScale(influence, weight);

        if (linkMode == LinkMode::Additive) {
            // not support now

            assert(false);
        } else {
            matrixAdd(boneDeformations[i], influence);
            boneWeights[i] += weight;
        }

        for (auto childBone : bone->getChildren()) {
            this->computeBone(dynamic_pointer_cast<BoneNode>(childBone), boneDeformations, boneWeights, meshGeometryMatrix, globalPositionMatrix);
        }
    }
}

void Skeleton::updateBoneVertex(BoneNodePtr bone, vector<Vec3> &boneVertex, Int index)
{
    Vec3 v = bone->getWorldMatrix().getTranslation();

    Vec3 vertex = boneVertex.at(index);
    vertex = v;

    for (auto childBone : bone->getChildren()) {
        index++;

        this->updateBoneVertex(dynamic_pointer_cast<BoneNode>(childBone), boneVertex, index);
    }
}

}
