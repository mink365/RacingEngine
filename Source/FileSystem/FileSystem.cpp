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

#define PATHSEPERATOR_CHAR				'/'

namespace re {

FileList FileSystem::listFiles(const std::string &path, const std::string &extension, bool sort, bool fullRelativePath)
{
    FileList list;

    StrList extensions;
    this->GetExtensionList(extension, extensions);

    this->GetFileList(path, extensions, list);

    if (sort) {
        // TODO: sort it
    }

    return list;
}

FileList FileSystem::listFilesTree(const std::string &path, const std::string &extension, bool sort)
{
    FileList list;

    StrList extensions;
    this->GetExtensionList(extension, extensions);

    this->GetFileListTree(path, extensions, list);

    if (sort) {
        // TODO: sort it
    }

    return list;
}

FilePtr FileSystem::openFile(const std::string &path, fsMode mode)
{
    // TODO: search and check it in android assert or osPath


    FilePtr file = this->CreateFile(path, FileType::Permanent);

    this->openFile(file, mode);

    return file;
}

void FileSystem::openFile(FilePtr &file, fsMode mode)
{
    switch (file->type) {
    case FileType::Permanent:
    {
        std::shared_ptr<FilePermanent> localFile = std::dynamic_pointer_cast<FilePermanent>(file);

        FILE* fp = OpenOSFile( localFile->fullPath.c_str(), "rb" );
        if ( !fp ) {
            return;
        }

        localFile->fp = fp;
        localFile->mode = mode;
        localFile->checkLength();

        break;
    }
    }
}

void FileSystem::closeFile(ConstFilePtr &file)
{

}

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

void FileSystem::GetExtensionList(const std::string& extension, StrList &extensionList) const
{
    int s, e, l;

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
    if ( searchPaths.size() ) {
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
        if ( search.type == SearchPathType::Permanent ) {
            std::string netpath = BuildOSPath( search.rootDir, relativePath );

            StrList sysFiles;

            for (int i = 0; i < extensions.size(); i++ ) {
                sysFiles.clear();

                // if we are searching for directories, remove . and ..
                if ( extensions[i][0] == '/' && extensions[i][1] == 0 ) {
                    // scan for files in the filesystem
                    ListOSDirectories( netpath, sysFiles );

//                    sysFiles.Remove( "." );
//                    sysFiles.Remove( ".." );

                    for(int j = 0; j < sysFiles.size(); j++ ) {
                        directories->push_back(sysFiles[j]);
                    }
                } else {
                    // scan for files in the filesystem
                    ListOSFiles( netpath, extensions[i], sysFiles );

                    for(int j = 0; j < sysFiles.size(); j++ ) {
                        FilePtr file = CreateFile(sysFiles[j], search.type);

                        list.push_back(file);
                    }
                }
            }
        } else if ( search.type == SearchPathType::PACK ) {

        } else if (search.type == SearchPathType::AndroidAsset) {

        }
    }

    return list.size();
}

int FileSystem::GetFileListTree(const std::string &relativePath, const StrList &extensions, FileList &list)
{
    int i;
    StrList slash, folders( 128 );

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

bool FileSystem::IsOSDirectory(const std::string path)
{
#ifndef __MWERKS__
#ifndef WIN32
    struct stat buf;
    if ( stat( path.c_str(), &buf ) == 0) {
        if (S_ISREG(buf.st_mode)) {
            return true;
        }
    } else {
        // error, file not find?
    }
#endif
#endif

    return false;
}

std::string FileSystem::BuildOSPath(const std::string &dir, const std::string &relativeDir)
{
    // TODO: in windows remove "\\"

    return dir + "/" + relativeDir;
}

void ExtractFileExtension( const std::string& path, std::string& ext ) {
    int pos = path.find_last_of(".");
    ext = path.substr(pos + 1, path.length());
}

int FileSystem::ListOSFiles(const std::string &directory, const std::string &extension, StrList &list)
{
    dirent* dp;

    auto dirp = opendir(directory.c_str());
    while ((dp = readdir(dirp)) != NULL) {
        std::string file = directory + "/" + dp->d_name;

        if (!IsOSDirectory(file.c_str())) {
            std::string ext;
            ExtractFileExtension(file, ext);

            if (ext == extension) {
                list.push_back(file);
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
    while ((dp = readdir(dirp)) != NULL) {
        std::string file = directory + "/" + dp->d_name;

        if (IsOSDirectory(file.c_str())) {
            list.push_back(file);
        }
    }
    closedir(dirp);

    return list.size();
}

FilePtr FileSystem::CreateFile(const std::string &path, FileType type)
{
    FilePtr file = nullptr;

    if (type == FileType::Permanent) {
        auto localFile = std::make_shared<FilePermanent>();
        localFile->fullPath = path;

        file = localFile;
    } else if (type == FileType::PACK) {

    } else if (type == FileType::AndroidAsset) {

    }

    return file;
}

FileSystem::FileSystem()
{
}

}
