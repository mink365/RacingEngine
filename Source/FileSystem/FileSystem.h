#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>

#include "File.h"
#include "Base/Singleton.h"
#include "platform.h"

namespace re {

/**
 * @brief The SearchPath struct
 *
 * use string to define the type of path
 * android asset dir: asset:Resources/
 * real path: /home/xxx/Resources
 * if we use PAK to pack our resource we may use it like this:
 * pack1:Images/, pack2:Images/ ....
 * and create a File type such as PakFile
 */
struct SearchPath {
    std::string dir;
};

typedef std::vector<std::string> StrList;
typedef std::vector<FilePtr> FileList;

void ExtractFileExtension( const std::string& path, std::string& ext );

class FileSystem
{
public:
    static FileSystem& getInstance();

    virtual ~FileSystem(){}

public:
    void addSearchPath(const SearchPath& searchPath);

    FilePtr getFile(const std::string& relativePath, fsMode mode=fsMode::Read);
    FileList listFiles(const std::string& relativePath, const std::string& extension="", bool sort=false, bool fullRelativePath=false);
    FileList listFilesTree(const std::string& relativePath, const std::string& extension="", bool sort=false);

protected:
    virtual FilePtr CreateFile(const std::string& path, uint32_t mode=(uint32_t)fsMode::Read);
    virtual bool FileExists(const std::string& path);

    void GetExtensionList(const std::string &extension, StrList &extensionList) const;
    int	GetFileList(const std::string& relativePath, const StrList &extensions, FileList &list, StrList* directories=nullptr);
    int	GetFileListTree( const std::string& relativePath, const StrList &extensions, FileList &list);

    std::string JoinPath(const std::string& dir, const std::string& relativeDir);

    FILE* OpenOSFile( const char *fileName, const char *mode);
    virtual bool IsOSDirectory(const std::string& path);
    virtual int ListOSFiles(const std::string& directory, const std::string& extension, StrList& list);
    virtual int ListOSDirectories(const std::string& directory, StrList& list);

private:
    std::vector<SearchPath> searchPaths;
};

}

#endif // FILESYSTEM_H
