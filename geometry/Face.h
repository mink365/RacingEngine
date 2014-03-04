#ifndef FACE_H
#define FACE_H

#include "platform.h"

class Face
{
public:
    uint a;
    uint b;
    uint c;

    Face(const uint a, const uint b, const uint c);

    void set(uint a, uint b, uint c);
};

#endif // FACE_H
