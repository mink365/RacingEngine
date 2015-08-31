#ifndef RE_UI_RENDERELEMENT_H
#define RE_UI_RENDERELEMENT_H

#include "Base/ECS/Component.h"

namespace re {

class Material;
class Texture;
class Geometry;

using MaterialPtr = SharedPtr<Material>;
using TexturePtr = SharedPtr<Texture>;
using GeometryPtr = SharedPtr<Geometry>;

namespace ui {

class CanvasRenderElement : public Component<CanvasRenderElement>
{
public:
    CanvasRenderElement();

    MaterialPtr getMaterial() const;
    void setMaterial(const MaterialPtr &value);

    TexturePtr getTexture() const;
    void setTexture(const TexturePtr &value);

    GeometryPtr getGeometry() const;
    void setGeometry(const GeometryPtr &value);

private:
    MaterialPtr material;
    TexturePtr texture;

    GeometryPtr geometry;
};

} // namespace ui
} // namespace re

#endif // RE_UI_RENDERELEMENT_H
