#ifndef FACE_H
#define FACE_H

#include "platform.h"

namespace re {

class Face
{
public:
    uint a;
    uint b;
    uint c;

    Face(const uint a, const uint b, const uint c);

    void set(uint a, uint b, uint c);
};

} // namespace re

#endif // FACE_H
