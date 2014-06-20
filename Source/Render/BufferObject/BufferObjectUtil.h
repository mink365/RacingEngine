#ifndef BUFFEROBJECTUTIL_H
#define BUFFEROBJECTUTIL_H

#include "Base/Singleton.h"
#include "Geometry/Geometry.h"

namespace re {

class BufferObjectUtil : public Singleton<BufferObjectUtil>
{
    friend class Singleton;
public:
    void loadGeometryToHardware(Geometry &geometry);

    void updateGeometryToHardware(Geometry &geometry);
private:
    BufferObjectUtil();

};

} // namespace re

#endif // BUFFEROBJECTUTIL_H
