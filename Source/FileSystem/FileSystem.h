#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>

#include "File.h"
#include "Base/Singleton.h"
#include "platform.h"

namespace re {

typedef FileType SearchPathType;

struct SearchPath {
    union {
        const char* rootDir;
    };

    std::string secondDir;

    SearchPathType type;
};

typedef std::vector<std::string> StrList;
typedef std::vector<FilePtr> FileList;

class FileSystem : public Singleton<FileSystem>
{
    friend class Singleton;

public:
    void addSearchPath(const SearchPath& searchPath);

    FileList listFiles(const std::string& relativePath, const std::string& extension="", bool sort=false, bool fullRelativePath=false);
    FileList listFilesTree(const std::string& relativePath, const std::string& extension="", bool sort=false);

    FilePtr openFile(const std::string& relativePath, fsMode mode=fsMode::Read);
    void openFile(FilePtr &file, fsMode mode=fsMode::Read);
    void closeFile(ConstFilePtr& file);

protected:
    FILE* OpenOSFile( const char *fileName, const char *mode);

    void GetExtensionList(const std::string &extension, StrList &extensionList ) const;
    int	GetFileList(const std::string& relativePath, const StrList &extensions, FileList &list, StrList* directories=nullptr);
    int	GetFileListTree( const std::string& relativePath, const StrList &extensions, FileList &list);

    bool IsOSDirectory(const std::string path);
    std::string BuildOSPath(const std::string& dir, const std::string& relativeDir);
    virtual int ListOSFiles(const std::string& directory, const std::string& extension, StrList& list);
    virtual int ListOSDirectories(const std::string& directory, StrList& list);

    FilePtr CreateFile(const std::string& path, FileType type);
private:
    FileSystem();

private:
    std::vector<SearchPath> searchPaths;
};

}

#endif // FILESYSTEM_H
