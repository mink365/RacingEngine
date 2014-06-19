#ifndef BUFFEROBJECTUTIL_H
#define BUFFEROBJECTUTIL_H

#include "Geometry/Geometry.h"

namespace re {

class BufferObjectUtil
{
public:
    static BufferObjectUtil& getInstance();

    void loadGeometryToHardware(Geometry &geometry);

    void updateGeometryToHardware(Geometry &geometry);
private:
    BufferObjectUtil();

};

} // namespace re

#endif // BUFFEROBJECTUTIL_H
