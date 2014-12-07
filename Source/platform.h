#ifndef PLATFORM_H
#define PLATFORM_H

#include <string>
#include <memory>
#include <assert.h>

#include "Base/Uncopyable.h"
#include "Base/Shared.h"
#include "Base/Singleton.h"

#include "Util/LogUtil.h"

#include "PredefineType.h"

#include "Util/PrintUtil.h"
#include "Util/StringUtil.h"

using namespace std;

typedef uint GPU_ID;

// TODO: 32 or 64
typedef long OBJ_ID;

typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;

typedef char byte;
typedef int64_t Long;
typedef int32_t Int;

enum ClearBufferBit {
    COLOR_BUFFER_BIT = 2,
    DEPTH_BUFFER_BIT = 4,
    STENCIL_BUFFER_BIT = 8,
};

#endif // PLATFORM_H
