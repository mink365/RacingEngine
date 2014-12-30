/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef ANDROIDFILESYSTEM_H
#define ANDROIDFILESYSTEM_H

#include "FileSystem/FileSystem.h"

#include <android/asset_manager.h>
#include <string.h>

namespace re
{

class FileSystemAndroid : public FileSystem
{
public:
    const std::string ASSET_KEY = "asset:";

public:
    FileSystemAndroid();
    ~FileSystemAndroid();

    void BindAssetManager(AAssetManager* assetManager);

protected:
    virtual FilePtr CreateFile(const std::string& netpath);
    virtual bool FileExists(const std::string& netpath);

    virtual bool IsOSDirectory(const std::string& netpath);
    virtual int ListOSFiles(const std::string& directory, const std::string& extension, StrList& list);
    virtual int ListOSDirectories(const std::string& directory, StrList& list);

private:
    FilePtr CreateAssetFile(const std::string& path);
    bool AssetFileExists(const std::string& path);

    bool IsAssetPath(const std::string& path);
    std::string GetPathInAsset(const std::string& path);

    bool IsAssetDirectory(const std::string& path);
    virtual int ListAssetFiles(const std::string& directory, const std::string& extension, StrList& list);
    virtual int ListAssetDirectories(const std::string& directory, StrList& list);

private:
    AAssetManager* _assetManager;
};

inline bool FileSystemAndroid::IsAssetPath(const std::string& path)
{
    if (strncmp(path.c_str(), ASSET_KEY.c_str(), ASSET_KEY.size()) == 0) {
        return true;
    }

    return false;
}

}

#endif // ANDROIDFILESYSTEM_H
