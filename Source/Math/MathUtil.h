/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <algorithm>

namespace re
{

template <typename T, int N>
struct max_minimize_helper
{
    static void DoMax(T const lhs[N], T const rhs[N], T out[N])
    {
        out[0] = std::max<T>(lhs[0], rhs[0]);
        max_minimize_helper<T, N - 1>::DoMax(lhs + 1, rhs + 1, out + 1);
    }

    static void DoMin(T const lhs[N], T const rhs[N], T out[N])
    {
        out[0] = std::min<T>(lhs[0], rhs[0]);
        max_minimize_helper<T, N - 1>::DoMin(lhs + 1, rhs + 1, out + 1);
    }
};
template <typename T>
struct max_minimize_helper<T, 1>
{
    static void DoMax(T const lhs[1], T const rhs[1], T out[1])
    {
        out[0] = std::max<T>(lhs[0], rhs[0]);
    }

    static void DoMin(T const lhs[1], T const rhs[1], T out[1])
    {
        out[0] = std::min<T>(lhs[0], rhs[0]);
    }
};

}

#endif // MATHUTIL_H

