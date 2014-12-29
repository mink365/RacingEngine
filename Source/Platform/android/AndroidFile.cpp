/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "AndroidFile.h"

namespace re
{

extern AAssetManager* __assetManager;

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
    close();
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

void AndroidFile::open()
{
    const char* filePath = fullPath.c_str();

    AAsset* asset = AAssetManager_open(__assetManager, filePath, AASSET_MODE_RANDOM);

    this->_asset = asset;

    this->fileSize = (size_t)AAsset_getLength(asset);
}

void AndroidFile::close()
{
    if (_asset) {
        AAsset_close(_asset);
    }
    _asset = NULL;
}

}
