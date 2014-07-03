#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>

#include "File.h"

namespace re {

enum class fsMode {
    Read = 0,
    Write = 1,
};

class FileSystem
{
public:
    FileSystem();

    std::vector<FilePtr> listFiles(const std::string& path, const std::string& extension, bool sort=false, bool fullRelativePath=false);
    std::vector<FilePtr> listFilesTree(const std::string& path, const std::string& extension, bool sort=false);

    FilePtr openFile(const std::string& path, fsMode mode=fsMode::Read);
    void closeFile(ConstFilePtr& file);
};

}

#endif // FILESYSTEM_H
