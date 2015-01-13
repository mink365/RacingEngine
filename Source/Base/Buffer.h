#ifndef RE_BUFFER_H
#define RE_BUFFER_H

#include "Shared.h"

#include <stddef.h>
#include <stdlib.h>

namespace re {

template<class T>
class Buffer
{
public:
    Buffer() {
        this->_data = nullptr;
        this->_size = 0;
    }

    Buffer(size_t capacity) {
        this->_data = nullptr;
        this->_size = 0;

        this->allocate(capacity);
    }

    ~Buffer() {
        this->clear();
    };

    void allocate(size_t capacity) {
        this->clear();

        this->_data = (T*)malloc(capacity * sizeof(T));
        this->_size = capacity;
    };
    void clear() {
        if (this->_data) {
            free(_data);
        }
        _data = nullptr;
        _size = 0;
    };

    T* getData() {
        return _data;
    };

    void setData(T* data, size_t size) {
        this->clear();

        this->_size = size;
        this->_data = data;
    }

    size_t getSize() {
        return _size;
    };

    size_t getByteSize() {
        return _size * sizeof(T);
    }

    bool isNull() {
        return (this->_size == 0) || (this->_data == nullptr);
    };
private:
    size_t _size;
    T* _data;
};

typedef Buffer<unsigned char> ByteBuffer;

} // namespace re

#endif // RE_BUFFER_H
