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

bool FileAndroid::canRead()
{
    return true;
}

bool FileAndroid::canWrite()
{
    return false;
}

bool FileAndroid::canSeek()
{
    return true;
}

size_t FileAndroid::read(void *buffer, size_t size, size_t count)
{
    size_t result = AAsset_read(_asset, buffer, size * count);
    return result > 0 ? ((size_t)result) / size : 0;
}

size_t FileAndroid::write(const void *buffer, size_t size, size_t count)
{
    return 0;
}

bool FileAndroid::eof()
{
    return position() >= length();
}

size_t FileAndroid::length()
{
    return (size_t)AAsset_getLength(_asset);
}

long int FileAndroid::position()
{
    return AAsset_getLength(_asset) - AAsset_getRemainingLength(_asset);
}

bool FileAndroid::seek(long int offset, int origin)
{
    return AAsset_seek(_asset, offset, origin) != -1;
}

bool FileAndroid::rewind()
{
    if (canSeek())
    {
        return AAsset_seek(_asset, 0, SEEK_SET) != -1;
    }
    return false;
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
