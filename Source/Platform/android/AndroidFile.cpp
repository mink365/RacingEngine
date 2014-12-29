/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "AndroidFile.h"

namespace re
{

AndroidFile::AndroidFile()
{
    name = "invalid";
    fullPath = "invalid";
    _asset = NULL;
    mode = 0;
    fileSize = 0;
}

AndroidFile::~AndroidFile()
{
    if (_asset) {
        AAsset_close(_asset);
    }
    _asset = NULL;
}

const std::string &AndroidFile::getName() const
{
    return name;
}

const std::string &AndroidFile::getFullPath() const
{
    return fullPath;
}

int AndroidFile::read(void *buffer, int len)
{
    int result = AAsset_read(_asset, ptr, size * count);
    return result > 0 ? ((size_t)result) / size : 0;
}

int AndroidFile::write(const void *buffer, int len)
{
    return 0;
}

size_t AndroidFile::length() const
{
    return (size_t)AAsset_getLength(_asset);
}

}
