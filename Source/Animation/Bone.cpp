#include "Bone.h"

#include "Scene/Node.h"
#include "Scene/Transform.h"
#include "Animation/AnimationTrack.h"

namespace re {

Bone::Bone()
    :type(0), linkMode(LinkMode::Normalize)
{
}

void Bone::setType(Int type)
{
    this->type = type;
}

void Bone::setLinkMode(LinkMode type)
{
    this->linkMode = type;
}

Int Bone::getNumLinkedControlPoints()
{
    return this->linkIndices.size();
}

void Bone::setAnimationTrack(AnimationTrackPtr track)
{
    this->animationTrack = track;
    track->entity = this->getEntity();
}

Mat4 Bone::getVertexTransformMatrix(const Mat4& meshGeometryMatrix, const Mat4& globalPositionMatrix)
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
        Mat4 clusterRelativeCurrentPositionInverse = globalCurrentPositionInverse * getComponent<Transform>()->getWorldMatrix();

        return clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;
    }
}

void Bone::updateMatrix()
{
    if (animationTrack != nullptr) {
        animationTrack->updateTimeInfo();

        animationTrack->updateLocalMatrix();
    }

    this->getComponent<Transform>()->refresh();
}

}
