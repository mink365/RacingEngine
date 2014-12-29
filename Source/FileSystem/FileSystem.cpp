#include "FileSystem.h"

#ifdef WIN32
    #include <io.h>	// for _read
#else
    #if !__MACH__ && __MWERKS__
        #include <types.h>
        #include <stat.h>
    #else
        #include <sys/types.h>
        #include <sys/stat.h>
        #include <dirent.h>
    #endif
    #include <unistd.h>
#endif

#include <algorithm>

#define PATHSEPERATOR_CHAR				'/'

namespace re {

FileSystem::FileSystem()
{
}

void FileSystem::addSearchPath(const SearchPath &searchPath)
{
    this->searchPaths.push_back(searchPath);
}

FilePtr FileSystem::getFile(const std::string &relativePath, fsMode mode)
{
    uint32_t _mode;
    if (mode == fsMode::Append) {
        _mode = (1 << (uint32_t)fsMode::Write) | (1 << (uint32_t)fsMode::Append);
    } else {
        _mode = (1 << (uint32_t)mode);
    }

    for (auto search : this->searchPaths) {
        std::string netpath = JoinPath( search.dir, relativePath );

        if (FileExists(netpath)) {
            FilePtr file = this->CreateFile(netpath);
            file->mode = _mode;
            return file;
        }
    }

    return nullptr;
}

FileList FileSystem::listFiles(const std::string &relativePath, const std::string &extension, bool sort, bool fullRelativePath)
{
    FileList list;

    StrList extensions;
    this->GetExtensionList(extension, extensions);

    this->GetFileList(relativePath, extensions, list);

    if (sort) {
        // TODO: sort it
    }

    return list;
}

FileList FileSystem::listFilesTree(const std::string &relativePath, const std::string &extension, bool sort)
{
    FileList list;

    StrList extensions;
    this->GetExtensionList(extension, extensions);

    this->GetFileListTree(relativePath, extensions, list);

    if (sort) {
        // TODO: sort it
    }

    return list;
}

void FileSystem::GetExtensionList(const std::string& extension, StrList &extensionList) const
{
    size_t s, e, l;

    l = extension.length();
    s = 0;
    while( 1 ) {
        e = extension.find_first_of('|', s);
        if ( e != std::string::npos ) {
            extensionList.push_back(extension.substr(s, e));
            s = e + 1;
        } else {
            extensionList.push_back(extension.substr(s, l));
            break;
        }
    }
}

int FileSystem::GetFileList(const std::string &relativePath, const StrList &extensions, FileList &list, StrList* directories)
{
    if ( !searchPaths.size() ) {
//        common->FatalError( "Filesystem call made without initialization\n" );
    }

    if ( !extensions.size() ) {
        return 0;
    }

    if ( !relativePath.size() ) {
        return 0;
    }

    int pathLength = relativePath.size();
    if ( pathLength ) {
        pathLength++;	// for the trailing '/'
    }

    // search through the path, one element at a time, adding to list
    for( auto search : searchPaths) {
        std::string netpath = JoinPath( search.dir, relativePath );

        StrList sysFiles;

        for (size_t i = 0; i < extensions.size(); i++ ) {
            sysFiles.clear();

            const std::string& extension = extensions[i];

            // if we are searching for directories, remove . and ..
            if ( extension[0] == '/' && extension[1] == 0 ) {
                // scan for directory in the filesystem
                ListOSDirectories( netpath, sysFiles );

                auto iter = std::remove_if(sysFiles.begin(), sysFiles.end(),
                [](const std::string& file) {
                    return file == "." || file == "..";
                });
                sysFiles.erase(iter, sysFiles.end());

                for(size_t j = 0; j < sysFiles.size(); j++ ) {
                    auto dir = JoinPath(relativePath, sysFiles[j]);
                    directories->push_back(dir);
                }
            } else {
                // scan for files in the filesystem
                ListOSFiles( netpath, extension, sysFiles );

                for(size_t j = 0; j < sysFiles.size(); j++ ) {
                    std::string path = JoinPath(netpath, sysFiles[j]);

                    FilePtr file = CreateFile(path);

                    list.push_back(file);
                }
            }
        }
    }

    return list.size();
}

int FileSystem::GetFileListTree(const std::string &relativePath, const StrList &extensions, FileList &list)
{
    size_t i;
    StrList slash, folders;

    // recurse through the subdirectories
    slash.push_back( "/" );
    GetFileList( relativePath, slash, list, &folders);

    for ( i = 0; i < folders.size(); i++ ) {
        if ( folders[i][0] == '.' ) {
            continue;
        }
        if ( folders[i] == relativePath ){
            continue;
        }
        GetFileListTree( folders[i], extensions, list);
    }

    // list files in the current directory
    GetFileList( relativePath, extensions, list);

    return list.size();
}

FilePtr FileSystem::CreateFile(const std::string &path)
{
    FilePtr file = nullptr;

    auto localFile = std::make_shared<FilePermanent>();

    int pos = path.find_last_of("/") + 1;
    localFile->name = path.substr(pos, path.length() - pos);
    localFile->fullPath = path;

    file = localFile;

    return file;
}

bool FileSystem::FileExists(const string &path)
{
    FILE* fp = this->OpenOSFile(path.c_str(), "rw");
    if ( fp ) {
        return true;
    }

    return false;
}

std::string FileSystem::JoinPath(const std::string &dir, const std::string &relativeDir)
{
    return dir + "/" + relativeDir;
}

void ExtractFileExtension( const std::string& path, std::string& ext ) {
    int pos = path.find_last_of(".");
    ext = path.substr(pos + 1, path.length());
}

// TODO: replace / to \\ when touch the real file
FILE *FileSystem::OpenOSFile(const char *fileName, const char *mode)
{
    FILE *fp;

    if (IsOSDirectory(fileName)) {
        return NULL;
    }

    fp = fopen( fileName, mode );

    if (!fp) {
        // TODO: error
    }

    return fp;
}

bool FileSystem::IsOSDirectory(const std::string& path)
{
#ifndef __MWERKS__
#ifndef WIN32
    struct stat buf;
    if ( stat( path.c_str(), &buf ) == 0) {
        if (buf.st_mode & S_IFDIR) {
            return true;
        }
    } else {
        // error, file not find?
    }
#endif
#endif

    return false;
}

int FileSystem::ListOSFiles(const std::string &directory, const std::string &extension, StrList &list)
{
    dirent* dp;

    auto dirp = opendir(directory.c_str());
    while (dirp && (dp = readdir(dirp)) != NULL) {
        std::string file = directory + "/" + dp->d_name;

        if (!IsOSDirectory(file.c_str())) {
            std::string ext;
            ExtractFileExtension(file, ext);

            if (ext == extension) {
                list.push_back(dp->d_name);
            }
        }
    }
    closedir(dirp);

    return list.size();
}

int FileSystem::ListOSDirectories(const std::string &directory, StrList &list)
{
    dirent* dp;

    auto dirp = opendir(directory.c_str());
    while (dirp && (dp = readdir(dirp)) != NULL) {
        std::string file = directory + "/" + dp->d_name;

        if (IsOSDirectory(file.c_str())) {
            list.push_back(dp->d_name);
        }
    }
    closedir(dirp);

    return list.size();
}

}
