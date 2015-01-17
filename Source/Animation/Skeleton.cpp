#include "Skeleton.h"

#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Bone.h"
#include "Scene/Node.h"
#include "Scene/Transform.h"


namespace re {

Skeleton::Skeleton()
    : linkMode(LinkMode::Normalize)
{
}

void Skeleton::setRootBone(BonePtr bone)
{
    this->rootBone = bone;

    this->linkMode = bone->linkMode;
}

BonePtr Skeleton::getRootBone() const
{
    return this->rootBone;
}

BonePtr Skeleton::getBone(Long id)
{
    this->bones.clear();
    this->cacheBones(this->rootBone);

    for (auto bone : this->bones) {
        if (bone->getId() == id) {
            return bone;
        }
    }

    return nullptr;
}

void Skeleton::cacheBones(BonePtr bone)
{
    this->bones.push_back(bone);

    for (auto& childBone : bone->getNode()->getChildren()) {
        this->cacheBones(childBone->getComponent<Bone>());
    }
}

void Skeleton::compute(vector<Mat4> &boneDeformations, vector<float> &boneWeights, const Mat4 &meshGeometryMatrix, const Mat4 &globalPositionMatrix)
{
    BonePtr rootBone = this->getRootBone();

    this->computeBone(rootBone, boneDeformations, boneWeights, meshGeometryMatrix, globalPositionMatrix);
}

void Skeleton::computeBone(BonePtr bone, vector<Mat4> &boneDeformations, vector<float> &boneWeights, const Mat4 &meshGeometryMatrix, const Mat4 &globalPositionMatrix)
{
    Mat4 vertexTransformMatrix = bone->getVertexTransformMatrix(meshGeometryMatrix, globalPositionMatrix);

    Int num = bone->getNumLinkedControlPoints();
    for (Int i = 0; i < num; ++i) {
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
        influence *= weight;

        if (linkMode == LinkMode::Additive) {
            // not support now
            assert(false);
        } else {
            boneDeformations[index] += influence;
            boneWeights[index] += weight;
        }
    }

    for (auto childBone : bone->getNode()->getChildren()) {
        this->computeBone(childBone->getComponent<Bone>(), boneDeformations, boneWeights, meshGeometryMatrix, globalPositionMatrix);
    }
}

void Skeleton::updateBoneVertex(BonePtr bone, vector<Vec3> &boneVertex, Int index)
{
    Vec3 v = bone->getComponent<Transform>()->getWorldMatrix().getTranslation();

    Vec3 vertex = boneVertex.at(index);
    vertex = v;

    for (auto childBone : bone->getNode()->getChildren()) {
        index++;

        this->updateBoneVertex(childBone->getComponent<Bone>(), boneVertex, index);
    }
}

}
