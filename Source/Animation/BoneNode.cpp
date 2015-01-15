#include "BoneNode.h"

#include "Scene/Node.h"
#include "Scene/Transform.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"

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

void BoneNode::setAnimationTrack(AnimationTrackPtr track)
{
    this->animationTrack = track;
    track->node = this->shared_from_this();
}

Mat4 BoneNode::getVertexTransformMatrix(const Mat4& meshGeometryMatrix, const Mat4& globalPositionMatrix)
{
    if (this->linkMode == LinkMode::Additive) {
        // TODO:
        assert(false);
        return Mat4();
    } else {
        this->updateMatrix();

        Mat4 globalInitPosition = transformMatrix * meshGeometryMatrix;

        Mat4 transformLinkMatrixInverse = transformLinkMatrix.inverse();
        Mat4 clusterRelativeInitPosition = transformLinkMatrixInverse * globalInitPosition;

        Mat4 globalCurrentPositionInverse = globalPositionMatrix.inverse();
        Mat4 clusterRelativeCurrentPositionInverse = globalCurrentPositionInverse * getTransform()->getWorldMatrix();

        return clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;
    }
}

void BoneNode::updateMatrix()
{
    if (animationTrack != nullptr) {
        animationTrack->updateTimeInfo();

        animationTrack->updateLocalMatrix();
    }

    this->getTransform()->refresh();
}

}
