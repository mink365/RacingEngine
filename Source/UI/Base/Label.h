#ifndef LABEL_H
#define LABEL_H

#include "Node2d.h"
#include "Font/Font.h"

namespace re {

class Label : public Node2d
{
public:
    Label();

    void setText(std::string& text);

protected:
    Font::ptr font;

    std::string text;
    bool dirtyFlag;
};

}

#endif // LABEL_H
