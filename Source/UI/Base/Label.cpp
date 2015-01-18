#include "Label.h"

#include "HierarchyColor.h"
#include "Transform2D.h"

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

    NodePtr node = this->getNode();

    string shaderName = font->getType() == FontType::TTF ? "Shader_Font" : "Shader_PTC";
    InitNodeForLeaf(node, font->getTexture(), shaderName);

    auto material = node->getComponent<Material>();
    material->getRenderState().blendState.blendModeAlpha = BlendMode::Alpha;
    material->getRenderState().blendState.blendModeRGB = BlendMode::Alpha;
}

void Label::setText(const string &text)
{
    auto mesh = this->getComponent<Mesh>();
    auto color = this->getComponent<HierarchyColor>();
    auto transform = this->getComponent<Transform2D>();

    mesh->getGeometry()->clear();

    TextStuffer::getInstance().AddText(std::wstring(text.begin(), text.end()), mesh->getGeometry(), this->font);

    const Rect& textRect = TextStuffer::getInstance().getTextRect();

    transform->setContentSize(textRect.size);
    // normal vertexOrigin is leftBottom of the rect, but label vertexOrigin is the pen begin place
//    transform->setAnchorPointInPixels(textRect.origin); // TODO: just an offset of vertext

    BufferObjectUtil::getInstance().loadGeometryToHardware(*(mesh.get()));
}

ComponentPtr Label::createCloneInstance() const
{
    return CreateCloneInstance<Label>();
}

void Label::copyProperties(const Component *component)
{
    Component::copyProperties(component);

    const Label* inst = dynamic_cast<const Label*>(component);
    if (inst) {
        this->font = inst->font;
        this->text = inst->text;
        this->dirtyFlag = true;
    }
}

}
