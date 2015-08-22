#ifndef FILE_H
#define FILE_H

#include <string>
#include <memory>
#include "PreDeclare.h"

namespace re {

class File;
class FilePermanent;

typedef SharedPtr<File> FilePtr;
typedef SharedPtr<const File> ConstFilePtr;

enum class fsMode : std::uint32_t
{
    Read = 1,
    Write = 2,
    Append = 4,
};

class File : public std::enable_shared_from_this<File>
{
    friend class FileSystem;

public:
    File();
    virtual ~File();

    virtual const std::string& getName() const = 0;
    /**
     * @brief getFullPath
     * @return
     *
     * full path is relative to the root of the resources
     */
    virtual const std::string& getFullPath() const = 0;
    virtual const std::string getExt() const;
    virtual const std::string getDirPath() const;

    ByteBufferPtr read();

    virtual void open() = 0;
    virtual void close() = 0;

    virtual bool canRead() = 0;
    virtual bool canWrite() = 0;
    virtual bool canSeek() = 0;

    virtual size_t read( void *buffer, size_t size, size_t count) = 0;
    virtual size_t write(const void *buffer, size_t size, size_t count) = 0;
    virtual size_t length() const;
    virtual bool eof() = 0;
    virtual int64_t position() = 0;
    virtual bool seek(int64_t offset, int32_t origin) = 0;
    virtual bool rewind() = 0;

protected:
    size_t fileSize;

    uint32_t mode;
};

class FilePermanent : public File
{
    friend class FileSystem;

public:
    FilePermanent();
    ~FilePermanent();

    virtual const std::string& getName() const;
    virtual const std::string& getFullPath() const;

    virtual bool canRead();
    virtual bool canWrite();
    virtual bool canSeek();

    virtual size_t read(void *buffer, size_t size, size_t count);
    virtual size_t write(const void *buffer, size_t size, size_t count);

    virtual bool eof();
    virtual int64_t position();
    virtual bool seek(int64_t offset, int32_t origin);
    virtual bool rewind();

    virtual void open();
    virtual void close();

protected:
    void checkLength();

private:
    std::string name;
    std::string fullPath;

    FILE *fp;
};

}

#endif // FILE_H
