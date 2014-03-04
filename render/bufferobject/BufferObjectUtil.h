#ifndef BUFFEROBJECTUTIL_H
#define BUFFEROBJECTUTIL_H

#include <geometry/Geometry.h>

class BufferObjectUtil
{
public:
    static BufferObjectUtil& getInstance();

    void loadGeometryToHardware(Geometry &geometry);
private:
    BufferObjectUtil();

};

#endif // BUFFEROBJECTUTIL_H
