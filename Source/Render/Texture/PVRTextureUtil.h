#ifndef RE_PVRTEXTUREUTIL_H
#define RE_PVRTEXTUREUTIL_H

#include "PVRTTexture.h"

#include <map>
#define CPVRTMap std::map

namespace re {

class PVRTextureUtil
{
public:
    PVRTextureUtil();
};

bool PVRTTextureLoadFromPVR(	const char * const filename,
                                    uint * const texName,
                                    const void *psTextureHeader,
                                    bool bAllowDecompress,
                                    const unsigned int nLoadFromLevel,
                                    CPVRTMap<unsigned int, CPVRTMap<unsigned int, MetaDataBlock> > *pMetaData);

} // namespace re

#endif // RE_PVRTEXTUREUTIL_H
