/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "FileAndroid.h"

namespace re
{

extern AAssetManager* __assetManager;

FileAndroid::FileAndroid()
{
    name = "invalid";
    fullPath = "invalid";
    _asset = NULL;
    mode = 0;
    fileSize = 0;
}

FileAndroid::~FileAndroid()
{
    close();
}

const std::string &FileAndroid::getName() const
{
    return name;
}

const std::string &FileAndroid::getFullPath() const
{
    return fullPath;
}

int FileAndroid::read(void *buffer, int len)
{
    int result = AAsset_read(_asset, ptr, size * count);
    return result > 0 ? ((size_t)result) / size : 0;
}

int FileAndroid::write(const void *buffer, int len)
{
    return 0;
}

void FileAndroid::open()
{
    const char* filePath = fullPath.c_str();

    AAsset* asset = AAssetManager_open(__assetManager, filePath, AASSET_MODE_RANDOM);

    this->_asset = asset;

    this->fileSize = (size_t)AAsset_getLength(asset);
}

void FileAndroid::close()
{
    if (_asset) {
        AAsset_close(_asset);
    }
    _asset = NULL;
}

}
