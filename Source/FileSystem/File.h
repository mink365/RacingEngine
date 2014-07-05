#ifndef FILE_H
#define FILE_H

#include <string>
#include <memory>
#include "Base/Buffer.h"

namespace re {

class File;
class FilePermanent;

typedef std::shared_ptr<File> FilePtr;
typedef std::shared_ptr<const File> ConstFilePtr;

enum class FileType {
    Permanent,
    PACK,
    AndroidAsset,
};

enum class fsMode {
    Read = 0,
    Write = 1,
    Append = 2,
};

class File : public std::enable_shared_from_this<File>
{
    friend class FileSystem;

public:
    File();
    virtual ~File();

    virtual const std::string& getName() const;
    /**
     * @brief getFullPath
     * @return
     *
     * full path is relative to the root of the resources
     */
    virtual const std::string& getFullPath() const;
    virtual const std::string getExt() const;

    Buffer::ptr read();
    virtual int read( void *buffer, int len );
    virtual int write(const void *buffer, int len);
    virtual int length() const;

protected:
    FileType type;

    int fileSize;
};

class FilePermanent : public File
{
    friend class FileSystem;

public:
    FilePermanent();
    ~FilePermanent();

    virtual const std::string& getName() const;
    virtual const std::string& getFullPath() const;

    virtual int read( void *buffer, int len );
    virtual int write(const void *buffer, int len);

protected:
    void checkLength();

private:
    std::string name;
    std::string fullPath;

    int mode;

    FILE *fp;
};

}

#endif // FILE_H