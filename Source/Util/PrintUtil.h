#ifndef PRINTUTIL_H
#define PRINTUTIL_H

#include <iostream>
#include <istream>
#include <sstream>

#include "Math/Vector.h"

namespace re {

template <typename T>
void PrintArray(std::string head, T *value, int count, int split) {
    std::ostringstream oss;

    oss << head << "Array: {" << "\n";
    for (int i = 0; i < count / split; ++i) {
        oss << "V[";
        for (int j = 0; j < split; ++j) {
            oss << value[i * split + j] << ", ";
        }

        oss << "]" << "\n";
    }

    LOG_D("%s", oss.str().c_str());
}

}

#endif // PRINTUTIL_H
