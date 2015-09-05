#include "Text.h"

#include "HierarchyColor.h"
#include "Transform2D.h"

#include "Font/TextStuffer.h"
#include "UI/Base/QuadStuffer.h"
#include "Math/Color.h"
#include "Render/BufferObject/BufferObjectUtil.h"
#include "Scene/RenderElement.h"
#include "Render/Mesh.h"
#include "PreDeclare.h"

namespace re {
namespace ui {

COMPONENT_DEPENDENCY(Text, CanvasRenderElement);

Text::Text()
    : Graphic()
{
}

void Text::setFont(Font::ptr &font)
{
    this->font = font;

    string shaderName = font->getType() == FontType::TTF ? "Shader_Font" : "Shader_PTC";

    auto element = this->getComponent<CanvasRenderElement>();
    auto material = CreateDefaultMaterial(font->getTexture(), shaderName);
    element->setMaterial(material);
    element->setTexture(font->getTexture());

    material->getRenderState().blendState.blendModeAlpha = BlendMode::Alpha;
    material->getRenderState().blendState.blendModeRGB = BlendMode::Alpha;
}

void Text::setText(const string &text)
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

void Text::onAwake()
{
    auto font = FontManager::instance().GetResource("default");
    this->setFont(font);
}

void Text::registerEvents()
{
    Graphic::registerEvents();
    RegisterEvent(Events::Awake, this, &Text::onAwake);
}

void Text::copyProperties(const Text &rhs)
{
    this->font = rhs.font;
    this->text = rhs.text;
    this->dirtyFlag = true;
}

} // namespace ui
} // namespace re
