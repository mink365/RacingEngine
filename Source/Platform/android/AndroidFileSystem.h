/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef ANDROIDFILESYSTEM_H
#define ANDROIDFILESYSTEM_H

#include "FileSystem/FileSystem.h"

#include <android/asset_manager.h>

namespace re
{

class AndroidFileSystem : public FileSystem
{
public:
    AndroidFileSystem();
    ~AndroidFileSystem();

protected:
    virtual FilePtr CreateFile(const std::string& path);
    virtual bool FileExists(const std::string& path);

    virtual bool IsOSDirectory(const std::string path);
    virtual int ListOSFiles(const std::string& directory, const std::string& extension, StrList& list);
    virtual int ListOSDirectories(const std::string& directory, StrList& list);

private:
    AAssetManager* _assetManager;
};

}

#endif // ANDROIDFILESYSTEM_H
