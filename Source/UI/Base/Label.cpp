#include "Label.h"

#include "Font/TextStuffer.h"
#include "UI/Base/QuadStuffer.h"
#include "Math/Color.h"
#include "Render/BufferObject/BufferObjectUtil.h"

namespace re {

Label::Label()
{
}

void Label::init(Font::ptr &font)
{
    this->font = font;

    SceneNodePtr node = std::dynamic_pointer_cast<SceneNode>(this->shared_from_this());
    InitNodeForLeaf(node, font->getTexture(), "Shader_Font");
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
