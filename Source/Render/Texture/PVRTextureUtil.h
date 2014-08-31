#ifndef RE_PVRTEXTUREUTIL_H
#define RE_PVRTEXTUREUTIL_H

#include "PVRTTexture.h"

#include <map>
#define CPVRTMap std::map

namespace re {

class Texture;

class PVRTextureUtil
{
public:
    static bool LoadFromPVR(Texture &texture, const void *psTextureHeader, const unsigned int nLoadFromLevel,
                            CPVRTMap<unsigned int, CPVRTMap<unsigned int, MetaDataBlock> > *pMetaData=nullptr);

};

} // namespace re

#endif // RE_PVRTEXTUREUTIL_H
