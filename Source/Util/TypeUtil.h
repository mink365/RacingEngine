#ifndef TYPEUTIL_H
#define TYPEUTIL_H

#include <type_traits>
#include <cxxabi.h>

inline bool IsBaseOf(void *src_ptr, const std::type_info& src_type, const std::type_info& dest_type) {
    void* ptr = __cxxabiv1::__dynamic_cast(src_ptr,
                                           (__cxxabiv1::__class_type_info*)&(src_type),
                                           (__cxxabiv1::__class_type_info*)&(dest_type), -1);

    if (ptr != nullptr) {
        return true;
    }

    return false;
}

#endif // TYPEUTIL_H
