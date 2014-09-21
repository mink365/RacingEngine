#ifndef BUFFEROBJECTUTIL_H
#define BUFFEROBJECTUTIL_H

#include "Base/Singleton.h"

namespace re {

class Mesh;

class BufferObjectUtil : public Singleton<BufferObjectUtil>
{
    friend class Singleton;
public:
    void loadGeometryToHardware(Mesh &geometry);

    void updateGeometryToHardware(Mesh &geometry);
private:
    BufferObjectUtil();

};

} // namespace re

#endif // BUFFEROBJECTUTIL_H
