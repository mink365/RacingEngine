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

class AndroidFile : public File
{
public:
    AndroidFile();
    ~AndroidFile();

    virtual const std::string& getName() const;
    virtual const std::string& getFullPath() const;

    virtual int read( void *buffer, int len );
    virtual int write(const void *buffer, int len);
    virtual size_t length() const;

private:
    std::string name;
    std::string fullPath;

    AAsset* _asset;
};

}

#endif // ANDROIDFILE_H
