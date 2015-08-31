#include "Label.h"

#include "HierarchyColor.h"
#include "Transform2D.h"

#include "Font/TextStuffer.h"
#include "UI/Base/QuadStuffer.h"
#include "Math/Color.h"
#include "Render/BufferObject/BufferObjectUtil.h"
#include "Scene/RenderElement.h"
#include "Render/Mesh.h"

namespace re {
namespace ui {

Label::Label()
    : Graphic()
{
}

void Label::init(Font::ptr &font)
{
    this->font = font;

    string shaderName = font->getType() == FontType::TTF ? "Shader_Font" : "Shader_PTC";

    this->getEntity()->addComponent<CanvasRenderElement>();
    auto element = this->getComponent<CanvasRenderElement>();
    auto material = CreateDefaultMaterial(font->getTexture(), shaderName);
    element->setMaterial(material);
    element->setTexture(font->getTexture());
    element->setGeometry(geometry);

    material->getRenderState().blendState.blendModeAlpha = BlendMode::Alpha;
    material->getRenderState().blendState.blendModeRGB = BlendMode::Alpha;
}

void Label::setText(const string &text)
{
    auto color = this->getComponent<HierarchyColor>();
    auto transform = this->getComponent<Transform2D>();

    geometry->clear();
    TextStuffer::instance().AddText(std::wstring(text.begin(), text.end()), geometry, this->font);

    const Rect& textRect = TextStuffer::instance().getTextRect();

    transform->setSize(textRect.size);
    // normal vertexOrigin is leftBottom of the rect, but label vertexOrigin is the pen begin place
//    transform->setAnchorInPixels(textRect.origin); // TODO: just an offset of vertext

}

void Label::copyProperties(const Label &rhs)
{
    this->font = rhs.font;
    this->text = rhs.text;
    this->dirtyFlag = true;
}

} // namespace ui
} // namespace re
