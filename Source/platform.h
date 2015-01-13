#ifndef PLATFORM_H
#define PLATFORM_H

#include <string>
#include <memory>
#include <assert.h>

#include "Base/Uncopyable.h"
#include "Base/Shared.h"
#include "Base/Singleton.h"
#include "Util/LogUtil.h"
#include "Util/StringUtil.h"
#include "PreDeclare.h"

using namespace std;

typedef uint8_t ubyte;
typedef uint16_t ushort;
typedef uint32_t uint;
typedef uint64_t ulong;

typedef int8_t byte;
typedef int32_t Int;
typedef int64_t Long;

typedef uint GPU_ID;
typedef uint64_t OBJ_ID;

enum ClearBufferBit {
    COLOR_BUFFER_BIT = 2,
    DEPTH_BUFFER_BIT = 4,
    STENCIL_BUFFER_BIT = 8,
};

// Assert macros.
#define _DEBUG

#ifdef _DEBUG
#define RE_ASSERT(expression) assert(expression)
#else
#define RE_ASSERT(expression)
#endif

#endif // PLATFORM_H
