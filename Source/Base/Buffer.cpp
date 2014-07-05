#include "Buffer.h"

#include <stdlib.h>

namespace re {

Buffer::Buffer(size_t capacity)
{
    this->_bytes = nullptr;
    this->_size = 0;

    this->allocate(capacity);
}

Buffer::~Buffer()
{
    this->clear();
}

void Buffer::allocate(size_t capacity)
{
    this->clear();

    this->_bytes = (unsigned char*)malloc(capacity);
    this->_size = capacity;
}

void Buffer::clear()
{
    if (this->_bytes) {
        free(_bytes);
    }
    _bytes = nullptr;
    _size = 0;
}

unsigned char *Buffer::getData()
{
    return this->_bytes;
}

size_t Buffer::getSize()
{
    return this->_size;
}

bool Buffer::isNull()
{
    return (this->_size == 0) || (this->_bytes == nullptr);
}

} // namespace re
