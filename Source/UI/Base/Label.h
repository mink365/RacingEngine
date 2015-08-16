#ifndef RE_UI_LABEL_H
#define RE_UI_LABEL_H

#include "Component.h"
#include "Font/Font.h"

namespace re {
namespace ui {

class Label : public Component
{
public:
    Label();

    void init(Font::ptr& font);

    void setText(const string &text);

protected:
    virtual ComponentPtr createCloneInstance() const;
    virtual void copyProperties(const Component* component) override;

protected:
    Font::ptr font;

    std::string text;
    bool dirtyFlag;
};

} // namespace ui
} // namespace re

#endif // RE_UI_LABEL_H
