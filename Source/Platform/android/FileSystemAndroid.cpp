/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "FileSystemAndroid.h"
#include "FileAndroid.h"

namespace re
{

AAssetManager* __assetManager;

FileSystem& FileSystem::getInstance()
{
    static FileSystemAndroid instance;

    return instance;
}

FileSystemAndroid::FileSystemAndroid()
{

}

FileSystemAndroid::~FileSystemAndroid()
{

}

void FileSystemAndroid::BindAssetManager(AAssetManager* assetManager)
{
    __assetManager = assetManager;
    this->_assetManager = assetManager;
}

FilePtr FileSystemAndroid::CreateFile(const string &netpath)
{
    if (IsAssetPath(netpath)) {
        auto _path = GetPathInAsset(netpath);
        return CreateAssetFile(_path);
    }

    return FileSystem::CreateFile(netpath);
}

bool FileSystemAndroid::FileExists(const string &netpath)
{
    if (IsAssetPath(netpath)) {
        auto _path = GetPathInAsset(netpath);
        return AssetFileExists(_path);
    }

    return FileSystem::FileExists(netpath);
}

bool FileSystemAndroid::IsOSDirectory(const string& netpath)
{
    if (IsAssetPath(netpath)) {
        auto _path = GetPathInAsset(netpath);
        return IsAssetDirectory(_path);
    }

    return FileSystem::IsOSDirectory(netpath);
}

int FileSystemAndroid::ListOSFiles(const string &directory, const string &extension, StrList &list)
{
    if (IsAssetPath(directory)) {
        auto _dir = GetPathInAsset(directory);
        return ListAssetFiles(_dir, extension, list);
    }

    return FileSystem::ListOSFiles(directory, extension, list);
}

int FileSystemAndroid::ListOSDirectories(const string &directory, StrList &list)
{
    if (IsAssetPath(directory)) {
        auto _dir = GetPathInAsset(directory);
        return ListAssetDirectories(_dir, list);
    }

    return FileSystem::ListOSDirectories(directory, list);
}

FilePtr FileSystemAndroid::CreateAssetFile(const string &path)
{
    auto localFile = std::make_shared<FileAndroid>();

    int pos = path.find_last_of("/") + 1;
    localFile->name = path.substr(pos, path.length() - pos);
    localFile->fullPath = path;

    return localFile;
}

bool FileSystemAndroid::AssetFileExists(const string &path)
{
    AAsset* asset = AAssetManager_open(_assetManager, path.c_str(), AASSET_MODE_RANDOM);
    if (asset)
    {
        size_t length = AAsset_getLength(asset);
        AAsset_close(asset);
        return length > 0;
    }
    return false;
}

string FileSystemAndroid::GetPathInAsset(const string &path)
{
    return path.substr(ASSET_KEY.size() + 1, path.size());
}

bool FileSystemAndroid::IsAssetDirectory(const string &path)
{
    AAssetDir* assetDir = AAssetManager_openDir(_assetManager, path.c_str());

    if (assetDir) {
        AAssetDir_close(assetDir);
        return true;
    }

    return false;
}

int FileSystemAndroid::ListAssetFiles(const string &directory, const string &extension, StrList &list)
{
    // List the files that are in the android APK at this path
    AAssetDir* assetDir = AAssetManager_openDir(_assetManager, directory.c_str());
    if (assetDir != NULL)
    {
        AAssetDir_rewind(assetDir);
        const char* file = NULL;
        while ((file = AAssetDir_getNextFileName(assetDir)) != NULL)
        {
            std::string ext;
            ExtractFileExtension(file, ext);

            if (ext == extension) {
                list.push_back(file);
            }
        }
        AAssetDir_close(assetDir);
    }

    return list.size();
}

int FileSystemAndroid::ListAssetDirectories(const string &directory, StrList &list)
{
    // can't get directory use native API
    return 0;
}

}
