#include "CanvasRenderElement.h"

namespace re {
namespace ui {

CanvasRenderElement::CanvasRenderElement()
{

}
MaterialPtr CanvasRenderElement::getMaterial() const
{
    return material;
}

void CanvasRenderElement::setMaterial(const MaterialPtr &value)
{
    material = value;
}
TexturePtr CanvasRenderElement::getTexture() const
{
    return texture;
}

void CanvasRenderElement::setTexture(const TexturePtr &value)
{
    texture = value;
}
GeometryPtr CanvasRenderElement::getGeometry() const
{
    return geometry;
}

void CanvasRenderElement::setGeometry(const GeometryPtr &value)
{
    geometry = value;
}




} // namespace ui
} // namespace re

