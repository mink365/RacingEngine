#include "BoneNode.h"

#include "scene/Node.h"
#include "animation/Animation.h"
#include "animation/Skeleton.h"

namespace re {

BoneNode::BoneNode()
    :type(0), linkMode(LinkMode::Normalize)
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

Mat4 BoneNode::getVertexTransformMatrix(const Mat4& meshGeometryMatrix, const Mat4& globalPositionMatrix)
{
    if (this->linkMode == LinkMode::Additive) {
        // TODO:

        assert(false);
    } else {
        this->updateMatrix();

        Mat4 globalInitPosition = transformMatrix * meshGeometryMatrix;

        Mat4 transformLinkMatrixInverse = transformLinkMatrix.invertOut();
        Mat4 clusterRelativeInitPosition = transformLinkMatrixInverse * globalInitPosition;

        Mat4 globalCurrentPositionInverse = globalPositionMatrix.invertOut();
        Mat4 clusterRelativeCurrentPositionInverse = globalCurrentPositionInverse * getWorldMatrix();

        return clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;
    }
}

void BoneNode::updateMatrix()
{
    if (animationTrack != nullptr) {
        animationTrack->updateTimeInfo();

        animationTrack->getLocalMatrix();
    }

    if (this->getParent() != nullptr) {
        this->worldMatrix = getParent()->getWorldMatrix() * this->getLocalMatrix();
    } else {
        this->worldMatrix = getLocalMatrix();
    }
}

}
