#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>

#include "File.h"
#include "Base/Singleton.h"
#include "platform.h"

namespace re {

struct SearchPath {
    std::string dir;
};

typedef std::vector<std::string> StrList;
typedef std::vector<FilePtr> FileList;

class FileSystem : public Singleton<FileSystem>
{
    friend class Singleton;

public:
    void addSearchPath(const SearchPath& searchPath);

    FilePtr getFile(const std::string& relativePath);
    FileList listFiles(const std::string& relativePath, const std::string& extension="", bool sort=false, bool fullRelativePath=false);
    FileList listFilesTree(const std::string& relativePath, const std::string& extension="", bool sort=false);

    void closeFile(FilePtr& file);

public:
    // some function for typed file
    void openFile(FilePermanent& file, fsMode mode=fsMode::Read);

protected:
    virtual FilePtr CreateFile(const std::string& path);
    virtual bool FileExists(const std::string& path);

    void GetExtensionList(const std::string &extension, StrList &extensionList) const;
    int	GetFileList(const std::string& relativePath, const StrList &extensions, FileList &list, StrList* directories=nullptr);
    int	GetFileListTree( const std::string& relativePath, const StrList &extensions, FileList &list);

    FILE* OpenOSFile( const char *fileName, const char *mode);
    bool IsOSDirectory(const std::string path);
    std::string BuildOSPath(const std::string& dir, const std::string& relativeDir);

    virtual int ListOSFiles(const std::string& directory, const std::string& extension, StrList& list);
    virtual int ListOSDirectories(const std::string& directory, StrList& list);
private:
    FileSystem();

private:
    std::vector<SearchPath> searchPaths;
};

}

#endif // FILESYSTEM_H
