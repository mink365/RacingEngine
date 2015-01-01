/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef ANDROIDFILE_H
#define ANDROIDFILE_H

#include "FileSystem/File.h"

#include <android/asset_manager.h>

namespace re
{

class FileAndroid : public File
{
    friend class FileSystemAndroid;

public:
    FileAndroid();
    ~FileAndroid();

    virtual const std::string& getName() const;
    virtual const std::string& getFullPath() const;

    virtual bool canRead();
    virtual bool canWrite();
    virtual bool canSeek();

    virtual size_t read(void *buffer, size_t size, size_t count);
    virtual size_t write(const void *buffer, size_t size, size_t count);

    virtual size_t length() const;
    virtual bool eof();
    virtual int64_t position();
    virtual bool seek(int64_t offset, int32_t origin);
    virtual bool rewind();

    virtual void open();
    virtual void close();

private:
    std::string name;
    std::string fullPath;

    AAsset* _asset;
};

}

#endif // ANDROIDFILE_H
