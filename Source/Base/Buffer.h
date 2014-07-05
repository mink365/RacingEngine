#ifndef RE_BUFFER_H
#define RE_BUFFER_H

#include "Shared.h"

#include <stddef.h>

namespace re {

class Buffer : public Shared<Buffer>
{
public:
    Buffer(size_t capacity);
    virtual ~Buffer();

    void allocate(size_t capacity);
    void clear();

    unsigned char* getData();
    size_t getSize();

    bool isNull();

private:
    size_t _size;
    unsigned char* _bytes;
};

} // namespace re

#endif // RE_BUFFER_H
