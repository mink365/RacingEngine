#include "BoneNode.h"

#include "scene/Node.h"
#include "animation/Animation.h"
#include "animation/Skeleton.h"

namespace re {

BoneNode::BoneNode()
{
}

void BoneNode::setType(Int type)
{
    this->type = type;
}

void BoneNode::setLinkMode(LinkMode type)
{
    this->linkMode = type;
}

Int BoneNode::getNumLinkedControlPoints()
{
    return this->linkIndices.size();
}

void BoneNode::updateLocalMatrix()
{
    if (animationTrack != nullptr) {
        animationTrack->getLocalMatrix();
    }
}

Mat4 &BoneNode::getGlobalMatrix()
{
    this->updateLocalMatrix();

    if (this->getParent() != nullptr) {
        this->worldMatrix = getParent()->getWorldMatrix() * this->getLocalMatrix();
    } else {
        this->worldMatrix = getLocalMatrix();
    }

    return this->worldMatrix;
}

Mat4 BoneNode::getVertexTransformMatrix(const Mat4& meshGeometryMatrix, const Mat4& globalPositionMatrix)
{
    if (this->linkMode == LinkMode::Additive) {
        // TODO:
    } else {
        Mat4 globalInitPosition = transformMatrix * meshGeometryMatrix;

        Mat4 transformLinkMatrixInverse = transformLinkMatrix.invertOut();
        Mat4 clusterRelativeInitPosition = transformLinkMatrixInverse * globalInitPosition;

        Mat4 globalCurrentPositionInverse = globalPositionMatrix.invertOut();
        Mat4 clusterRelativeCurrentPositionInverse = globalCurrentPositionInverse * getGlobalMatrix();

        return clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;
    }
}

}
