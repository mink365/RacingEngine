#ifndef BUFFEROBJECTUTIL_H
#define BUFFEROBJECTUTIL_H

#include "Base/Singleton.h"

namespace re {

class MeshData;

class BufferObjectUtil : public Singleton<BufferObjectUtil>
{
    friend class Singleton;
public:
    void loadGeometryToHardware(MeshData &meshData);

    void updateGeometryToHardware(const MeshData &meshData);
private:
    BufferObjectUtil();
};

} // namespace re

#endif // BUFFEROBJECTUTIL_H
