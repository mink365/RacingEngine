#include "ComponentFactory.h"

#include "UI/Base/HierarchyColor.h"
#include "UI/Base/Transform2D.h"
#include "UI/Base/Label.h"
#include "UI/Base/Sprite.h"
#include "UI/Base/NinePatch.h"

namespace re {

NodePtr CreateNode()
{
    auto node = Create<Node>();

    return node;
}

template<>
inline LabelPtr CreateComponent<Label>(FontPtr font)
{
    return CreateNode2DComponent<Label>(font);
}

template<>
inline SpritePtr CreateComponent<Sprite>(string path)
{
    return CreateNode2DComponent<Sprite>(path);
}

} // namespace re

