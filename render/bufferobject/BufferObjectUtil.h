#ifndef BUFFEROBJECTUTIL_H
#define BUFFEROBJECTUTIL_H

#include <geometry/Geometry.h>

namespace re {

class BufferObjectUtil
{
public:
    static BufferObjectUtil& getInstance();

    void loadGeometryToHardware(Geometry &geometry);
private:
    BufferObjectUtil();

};

} // namespace re

#endif // BUFFEROBJECTUTIL_H
