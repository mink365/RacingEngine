#ifndef LABEL_H
#define LABEL_H

#include "Component.h"
#include "Font/Font.h"

namespace re {

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

}

#endif // LABEL_H
