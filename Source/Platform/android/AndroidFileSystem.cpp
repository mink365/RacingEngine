/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "AndroidFileSystem.h"

namespace re
{

AndroidFileSystem::AndroidFileSystem()
{

}

FilePtr AndroidFileSystem::CreateFile(const string &netpath)
{
    if (IsAssetPath(netpath)) {
        auto _path = GetPathInAsset(netpath);
        return CreateAssetFile(_path);
    }

    return FileSystem::CreateFile(netpath);
}

bool AndroidFileSystem::FileExists(const string &netpath)
{
    if (IsAssetPath(netpath)) {
        auto _path = GetPathInAsset(netpath);
        return AssetFileExists(_path);
    }

    return FileSystem::FileExists(netpath);
}

bool AndroidFileSystem::IsOSDirectory(const string& netpath)
{
    if (IsAssetPath(netpath)) {
        auto _path = GetPathInAsset(netpath);
        return IsAssetDirectory(_path);
    }

    return FileSystem::IsOSDirectory(netpath);
}

int AndroidFileSystem::ListOSFiles(const string &directory, const string &extension, StrList &list)
{
    if (IsAssetPath(directory)) {
        auto _dir = GetPathInAsset(directory);
        return ListAssetFiles(_dir, extension, list);
    }

    return FileSystem::ListOSFiles(directory, extension, list);
}

int AndroidFileSystem::ListOSDirectories(const string &directory, StrList &list)
{
    if (IsAssetPath(directory)) {
        auto _dir = GetPathInAsset(directory);
        return ListAssetDirectories(_dir, list);
    }

    return FileSystem::ListOSDirectories(directory, list);
}

FilePtr AndroidFileSystem::CreateAssetFile(const string &path)
{

}

bool AndroidFileSystem::AssetFileExists(const string &netpath)
{

}

string AndroidFileSystem::GetPathInAsset(const string &path)
{
    return path.substr(AssetKey.size() + 1, path.size());
}

bool AndroidFileSystem::IsAssetDirectory(const string &path)
{

}

int AndroidFileSystem::ListAssetFiles(const string &directory, const string &extension, StrList &list)
{

}

int AndroidFileSystem::ListAssetDirectories(const string &directory, StrList &list)
{

}

}
