#ifndef FILE_H
#define FILE_H

#include <string>
#include <memory>

namespace re {

class File;
class FilePermanent;

typedef std::shared_ptr<File> FilePtr;
typedef std::shared_ptr<const File> ConstFilePtr;

class File
{
public:
    File();
    virtual ~File();

    virtual const char* getName() const;
    virtual const char* getFullPath() const;

    virtual int read( void *buffer, int len );
    virtual int write(const void *buffer, int len);
    virtual int length() const;
};

class FilePermanent : public File
{
public:
    virtual const char* getName() const;
    virtual const char* getFullPath() const;

    virtual int read( void *buffer, int len );
    virtual int write(const void *buffer, int len);
    virtual int length() const;

private:
    std::string name;
    std::string fullPath;

    int fileSize;

    FILE *file;
};

}

#endif // FILE_H
