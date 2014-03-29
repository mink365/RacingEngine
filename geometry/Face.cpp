#include "Face.h"

namespace re {

Face::Face(const uint a, const uint b, const uint c)
{
    this->a = a;
    this->b = b;
    this->c = c;
}

void Face::set(uint a, uint b, uint c)
{
    this->a = a;
    this->b = b;
    this->c = c;
}

}
