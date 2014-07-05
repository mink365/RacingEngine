#ifndef LABEL_H
#define LABEL_H

#include "Node2d.h"
#include "Font/Font.h"

namespace re {

class Label : public Node2d
{
public:
    Label();

    void init(Font::ptr& font);

    void setText(const string &text);

protected:
    Font::ptr font;

    std::string text;
    bool dirtyFlag;
};

}

#endif // LABEL_H
