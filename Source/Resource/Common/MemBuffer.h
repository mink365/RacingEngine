#ifndef MEMBUFFER_H
#define MEMBUFFER_H

#include <iostream>

namespace re {
class membuf : public std::basic_streambuf<char>
{
public:
  membuf(char* p, size_t n) {
    setg(p, p, p + n);
    setp(p, p + n);
  }
};
}

#endif // MEMBUFFER_H
