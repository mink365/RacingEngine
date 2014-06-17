#ifndef CONTAINERUTIL_H
#define CONTAINERUTIL_H

#include <vector>

namespace re {
    template<typename T>
    void VectorCopy(const T* array, int begin, int size, std::vector<T>& output) {
        int end = begin + size;

        output.reserve(size);

        for (int i = begin; i < end; ++i) {
            output.push_back(array[i]);
        }
    }
}

#endif // CONTAINERUTIL_H
