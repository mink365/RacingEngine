#include "Node2d.h"

#include "Scene/Mesh.h"

#include "Render/BufferObject/BufferObjectUtil.h"
#include "Shader/ShaderManager.h"

namespace re {

Node2d::Node2d()
{
}

Node2d::ptr Node2d::getParent() const
{
    return dynamic_pointer_cast<Node2d>(SceneNode::getParent());
}

void Node2d::removeFromParent()
{
    if (this->getParent()) {
        this->getParent()->removeChild(this->shared_from_this());
    }
}

void Node2d::onEnter()
{

}

void Node2d::onExit()
{

}

bool Node2d::isRunning()
{

}

bool Node2d::isVisible()
{
    return this->getVisible();
}

void Node2d::stopAllActions()
{
    // TODO: no need
}

void Node2d::setPosition(const Vec2 &position)
{
    this->localTranslation.set(position.x, position.y, this->localTranslation.z);

    this->markLocalTransformRefreshFlag();
}

Vec2 Node2d::getPosition() const
{
    return Vec2(localTranslation.x, localTranslation.y);
}

void Node2d::setPosition(float x, float y)
{
    localTranslation.x = x;
    localTranslation.y = y;

    this->markLocalTransformRefreshFlag();
}

void Node2d::setPositionX(float v)
{
    localTranslation.x = v;

    this->markLocalTransformRefreshFlag();
}

void Node2d::setPositionY(float v)
{
    localTranslation.y = v;

    this->markLocalTransformRefreshFlag();
}

float Node2d::getPositionX() const
{
    return localTranslation.x;
}

float Node2d::getPositionY() const
{
    return localTranslation.y;
}

void Node2d::setScale(const Vec2 &scale)
{
    localScaling.x = scale.x;
    localScaling.y = scale.y;

    this->markLocalTransformRefreshFlag();
}

Vec2 Node2d::getScale() const
{
    return Vec2(localScaling.x, localScaling.y);
}

void Node2d::setScale(float x, float y)
{
    localScaling.x = x;
    localScaling.y = y;

    this->markLocalTransformRefreshFlag();
}

void Node2d::setScale(float v)
{
    this->setScale(v, v);
}

void Node2d::setScaleX(float v)
{
    localScaling.x = v;

    this->markLocalTransformRefreshFlag();
}

void Node2d::setScaleY(float v)
{
    localScaling.y = v;

    this->markLocalTransformRefreshFlag();
}

float Node2d::getScaleX() const
{
    return localScaling.x;
}

float Node2d::getScaleY() const
{
    return localScaling.y;
}

void Node2d::setRotation(float v)
{
    Vec3 r(0, 0, v * DEG_TO_RAD);
    localRotation.fromAngles(r);

    this->markLocalTransformRefreshFlag();
}

float Node2d::getRotation() const
{
    return localRotation.toVec3().z * RAD_TO_DEG;
}

void Node2d::setAnchorPoint(const Vec2 &v)
{
    if (v == this->anchorPoint) {
        return;
    }

    this->anchorPoint = v;

    this->anchorPointInPoints = Vec2(size.width * anchorPoint.x, size.height * anchorPoint.y);

    this->markLocalTransformRefreshFlag();
}

const Vec2 &Node2d::getAnchorPoint() const
{
    return this->anchorPoint;
}

Vec2 Node2d::getAnchorPointInPixels() const
{
    float x = this->anchorPoint.x * size.width;
    float y = this->anchorPoint.y * size.height;

    return Vec2(x, y);
}

void Node2d::setContentSize(const Size &size)
{
    this->size = size;

    this->anchorPointInPoints = Vec2(size.width * anchorPoint.x, size.height * anchorPoint.y);

    this->markLocalTransformRefreshFlag();
}

const Size &Node2d::getContentSize() const
{
    return this->size;
}

Rect Node2d::getBoundingBox() const
{
    // TODO:
}

Vec2 Node2d::convertToNodeSpace(const Vec2 &worldPoint) const
{
    Vec3 v(worldPoint.x, worldPoint.y, 0);

    Vec3 result = this->getWorldMatrix().inverse() * v;

    return Vec2(result.x, result.y);
}

Vec2 Node2d::convertToWorldSpace(const Vec2 &nodePoint) const
{
    Vec3 v(nodePoint.x, nodePoint.y, 0);

    Vec3 result = this->getWorldMatrix() * v;

    return Vec2(result.x, result.y);
}

Geometry::ptr Node2d::getGeometry() const
{
    if (this->attribute != nullptr && this->attribute->getType() == NodeAttributeType::Mesh) {
        auto mesh = std::dynamic_pointer_cast<Mesh>(this->attribute);

        return mesh->getGeometry();
    }

    return nullptr;
}

void Node2d::updateLocalMatrix()
{
    Vec3 position = this->localTranslation;

    bool needsSkewMatrix = (skew == Vec2::Zero);

    if (needsSkewMatrix && !(anchorPointInPoints == Vec2::Zero)) {
        Vec3 rotate = localRotation.toVec3();

        // TODO: use quat to make it
        // TODO: _rotationZ_X _rotationZ_Y
        float cx = 1, sx = 0, cy = 1, sy = 0;
        if (rotate.z || rotate.z)
        {
            float radiansX = -rotate.z;
            float radiansY = -rotate.z;
            cx = cosf(radiansX);
            sx = sinf(radiansX);
            cy = cosf(radiansY);
            sy = sinf(radiansY);
        }

        position.x += cy * -anchorPointInPoints.x * localScaling.x + -sx * -anchorPointInPoints.y * localScaling.y;
        position.y += sy * -anchorPointInPoints.x * localScaling.x +  cx * -anchorPointInPoints.y * localScaling.y;
    }

    this->localMatrix.fromRTS(localRotation, localTranslation, localScaling);

    if (needsSkewMatrix)
    {
        Mat4 skewMatrix(1, (float)tanf(DegreeToRadian(skew.y)), 0, 0,
                          (float)tanf(DegreeToRadian(skew.x)), 1, 0, 0,
                          0,  0,  1, 0,
                          0,  0,  0, 1);

        localMatrix = localMatrix * skewMatrix;

        // adjust anchor point
        if (!(anchorPointInPoints == Vec2::Zero))
        {
            // XXX: Argh, Mat4 needs a "translate" method.
            // XXX: Although this is faster than multiplying a vec4 * mat4
            localMatrix[3][0] += localMatrix[0][0] * -anchorPointInPoints.x + localMatrix[1][0] * -anchorPointInPoints.y;
            localMatrix[3][1] += localMatrix[0][1] * -anchorPointInPoints.x + localMatrix[1][1] * -anchorPointInPoints.y;
        }
    }

    this->refreshFlags &= ~RF_LOCAL_TRANSFORM;
    this->markWorldTransformRefreshFlag();
}

void Node2d::updateColor()
{
    worldColor = color * this->getParent()->getDisplayColor();

    for (auto child : this->children) {
        std::shared_ptr<Node2d> node = std::dynamic_pointer_cast<Node2d>(child);

        if (node) {
            node->updateColor();
        }
    }
}

void Rgba::setAlpha(float a)
{
    this->color.a = a;

    this->updateColor();
}

float Rgba::getAlpha() const
{
    return this->color.a;
}

void Rgba::setColor(const Color &color)
{
    this->color = color;

    // TODO: use markDirty to avoid it
    this->updateColor();
}

const Color &Rgba::getColor() const
{
    return this->color;
}

const Color &Rgba::getDisplayColor() const
{
    return this->worldColor;
}

void InitNodeForLeaf(SceneNodePtr &node, Texture::ptr texture, const std::string& shaderName)
{
    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->init();

    TextureUnitState &unit = mesh->getMaterial()->getTexture();
    unit.setUVstate(0, 0, 1, 1, 0);

    unit.addTextureFrame(texture);

    Geometry::ptr geometry = mesh->getGeometry();
    BufferObjectUtil::getInstance().loadGeometryToHardware(*(geometry.get()));

    Shader::ptr shader = ShaderManager::getInstance().getShader(shaderName);
    mesh->getMaterial()->setShder(shader);

    node->setNodeAttribute(mesh->clone());
}

}
