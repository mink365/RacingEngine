#include "Label.h"

#include "Font/TextStuffer.h"
#include "UI/Base/QuadStuffer.h"
#include "Math/Color.h"
#include "Render/BufferObject/BufferObjectUtil.h"
#include "Scene/Mesh.h"

namespace re {

Label::Label()
{
}

void Label::init(Font::ptr &font)
{
    this->font = font;

    SceneNodePtr node = std::dynamic_pointer_cast<SceneNode>(this->shared_from_this());

    string shaderName = font->getType() == FontType::TTF ? "Shader_Font" : "Shader_PTC";
    InitNodeForLeaf(node, font->getTexture(), shaderName);

    auto mesh = node->getComponent<Mesh>();
    MaterialPtr material = mesh->getMaterial();
    material->getRenderState().blendState.blendModeAlpha = BlendMode::Alpha;
    material->getRenderState().blendState.blendModeRGB = BlendMode::Alpha;
}

void Label::setText(const string &text)
{
    this->getGeometry()->clear();

    TextStuffer::getInstance().AddText(std::wstring(text.begin(), text.end()), this->getGeometry(), this->font);

    this->setContentSize(TextStuffer::getInstance().getTextRect().size);
    // normal vertexOrigin is leftBottom of the rect, but label vertexOrigin is the pen begin place
    this->anchorPointInPoints += TextStuffer::getInstance().getTextRect().origin;

    BufferObjectUtil::getInstance().loadGeometryToHardware(*(this->getMesh().get()));
}

NodePtr Label::createCloneInstance() const
{
    return CreateCloneInstance<Label>();
}

void Label::copyProperties(const Node *node)
{
    Node2d::copyProperties(node);

    const Label* inst = dynamic_cast<const Label*>(node);
    if (inst) {
        this->font = inst->font;
        this->text = inst->text;
        this->dirtyFlag = true;
    }
}

}
