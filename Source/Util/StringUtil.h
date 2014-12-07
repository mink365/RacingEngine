#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>

class StringUtil
{
public:
    StringUtil();

    static std::string Printf(const char * pszFormat, ...);
};

#endif // STRINGUTIL_H
