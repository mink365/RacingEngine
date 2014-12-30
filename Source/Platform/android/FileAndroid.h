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

    virtual size_t read( void *buffer, size_t len );
    virtual size_t write(const void *buffer, size_t len);

    virtual void open();
    virtual void close();

private:
    std::string name;
    std::string fullPath;

    AAsset* _asset;
};

}

#endif // ANDROIDFILE_H
