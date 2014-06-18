#include "Skeleton.h"

#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "BoneNode.h"
#include "Scene/Node.h"

namespace re {

Skeleton::Skeleton()
    : linkMode(LinkMode::Normalize)
{
}

void Skeleton::setRootBone(BoneNodePtr bone)
{
    this->rootBone = bone;

    this->linkMode = bone->linkMode;
}

BoneNodePtr Skeleton::getRootBone() const
{
    return this->rootBone;
}

BoneNodePtr Skeleton::getBone(Long id)
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

//    for (int i = 0; i < boneDeformations.size(); ++i) {
//        Mat4& mat = boneDeformations[i];

//        PrintArray("Mat: ", mat.toFloatPtr(), 16, 4);
//    }
}

void Skeleton::computeBone(BoneNodePtr bone, vector<Mat4> &boneDeformations, vector<float> &boneWeights, const Mat4 &meshGeometryMatrix, const Mat4 &globalPositionMatrix)
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

    for (auto childBone : bone->getChildren()) {
        this->computeBone(dynamic_pointer_cast<BoneNode>(childBone), boneDeformations, boneWeights, meshGeometryMatrix, globalPositionMatrix);
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
