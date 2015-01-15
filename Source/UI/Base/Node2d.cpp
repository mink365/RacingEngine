#include "Node2d.h"

#include "Scene/Mesh.h"

#include "Render/BufferObject/BufferObjectUtil.h"
#include "Shader/ShaderManager.h"
#include "Render/RenderQueue.h"

namespace re {

Node2d::Node2d()
    :inScene(false)
{
}

Node2d::ptr Node2d::getParent() const
{
    return dynamic_pointer_cast<Node2d>(Node::getParent());
}

void Node2d::removeFromParent()
{
    if (this->getParent()) {
        this->getParent()->removeChild(this->shared_from_this());
    }
}

void Node2d::onEnter()
{
    this->inScene = true;

    for (auto& node : this->children) {
        auto childNode = std::dynamic_pointer_cast<Node2d>(node);
        if (childNode) {
            childNode->onEnter();
        }
    }
}

void Node2d::onExit()
{
    this->inScene = false;

    for (auto& node : this->children) {
        auto childNode = std::dynamic_pointer_cast<Node2d>(node);
        if (childNode) {
            childNode->onExit();
        }
    }
}

bool Node2d::isRunning()
{
    return true;
}

bool Node2d::isVisible()
{
    return this->isVisible();
}

void Node2d::stopAllActions()
{
    // TODO: no need
}

void Node2d::setAnchorPoint(const Vec2 &v)
{
    if (v == this->anchorPoint) {
        return;
    }

    this->anchorPoint = v;

    this->anchorPointInPoints = Vec2(size.width * anchorPoint.x, size.height * anchorPoint.y);

    transform->markLocalTransformRefreshFlag();
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

    this->getTransform()->markLocalTransformRefreshFlag();
}

const Size &Node2d::getContentSize() const
{
    return this->size;
}

Rect Node2d::getBoundingBox() const
{
    Rect rect = Rect(0, 0, size.width, size.height);
    return RectApplyMatrix(rect, this->getTransform()->localMatrix);
}

Transform2DPtr Node2d::getTransform2D()
{
    std::dynamic_pointer_cast<Transform2D>(this->transform);
}

void Node2d::addChild(NodePtr node, Int index)
{
    Node::addChild(node, index);

    if (inScene) {
        auto childNode = std::dynamic_pointer_cast<Node2d>(node);
        if (childNode) {
            childNode->onEnter();
        }
    }
}

void Node2d::removeChild(NodePtr node)
{
    Node::removeChild(node);

    if (inScene) {
        auto childNode = std::dynamic_pointer_cast<Node2d>(node);
        if (childNode) {
            childNode->onExit();
        }
    }
}

void Node2d::removeAllChildren()
{
    if (inScene) {
        for (auto& node : this->children) {
            auto childNode = std::dynamic_pointer_cast<Node2d>(node);
            if (childNode) {
                childNode->onExit();
            }
        }
    }

    Node::removeAllChildren();
}

Geometry::ptr Node2d::getGeometry()
{
    auto mesh = this->getMesh();
    if (mesh) {
        return mesh->getGeometry();
    }

    return nullptr;
}

MeshPtr Node2d::getMesh()
{
    return this->getComponent<Mesh>();
}

void Node2d::updateColor()
{
    if (this->getParent()) {
        worldColor = color * this->getParent()->getDisplayColor();
    } else {
        worldColor = color;
    }

    this->updateViewColor();

    for (auto child : this->children) {
        std::shared_ptr<Node2d> node = std::dynamic_pointer_cast<Node2d>(child);

        if (node) {
            node->updateColor();
        }
    }
}

void Node2d::updateViewColor()
{
}

NodePtr Node2d::createCloneInstance() const
{
    return CreateCloneInstance<Node2d>();
}

void Node2d::copyProperties(const Node *node)
{
    Node::copyProperties(node);

    const Node2d* inst = dynamic_cast<const Node2d*>(node);
    if (inst) {
        this->size = inst->size;
        this->anchorPoint = inst->anchorPoint;
        this->anchorPointInPoints = inst->anchorPointInPoints;
        this->skew = inst->skew;
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

void InitNodeForLeaf(NodePtr &node, Texture::ptr texture, const std::string& shaderName)
{
    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->init();

    mesh->setGeometry(Geometry::create());

    MaterialPtr material = mesh->getMaterial();
    material->setQueueID(RENDER_QUEUE_UI);
    material->getRenderState().depthState.depthTestEnable = false;
    material->getRenderState().depthWrite = false;
    material->getRenderState().depthState.function = TestFunction::LessOrEqual;
    if (texture) {
        TextureUnitState::ptr unit = mesh->getMaterial()->getPass(0)->getTextureUnit(0);
        unit->setUVstate(0, 0, 1, 1, 0);
        unit->setTexture(texture);
    }

    BufferObjectUtil::getInstance().loadGeometryToHardware(*(mesh.get()));

    Shader::ptr shader = ShaderManager::getInstance().getShader(shaderName);
    mesh->getMaterial()->setShder(shader);

    node->addComponent(mesh);
}

}
