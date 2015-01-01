#include "File.h"

#include "FileSystem.h"

namespace re {

File::File()
{
    this->fileSize = -1;
}

File::~File()
{

}

size_t File::length() const
{
    return this->fileSize;
}

const std::string File::getExt() const
{
    const std::string& path = this->getFullPath();

    int pos = path.find_last_of(".");
    std::string ext = path.substr(pos + 1, path.length());

    return ext;
}

const std::string File::getDirPath() const
{
    const std::string& path = this->getFullPath();

    size_t pos = path.find_last_of("/");
    if (pos != std::string::npos) {
        std::string dir = path.substr(0, pos);

        return dir;
    } else {
        return "/";
    }
}

ByteBufferPtr File::read()
{
    this->open();

    ByteBufferPtr buf = std::make_shared<ByteBuffer>(this->length());

    this->read(buf->getData(), 1, this->length());

    return buf;
}

FilePermanent::FilePermanent()
{
    name = "invalid";
    fullPath = "invalid";
    fp = NULL;
    mode = 0;
    fileSize = 0;
}

FilePermanent::~FilePermanent()
{
    close();
}

const std::string &FilePermanent::getName() const
{
    return this->name;
}

const std::string &FilePermanent::getFullPath() const
{
    return this->fullPath;
}

bool FilePermanent::canRead()
{
    return true;
}

bool FilePermanent::canWrite()
{
    return true;
}

bool FilePermanent::canSeek()
{
    return true;
}

size_t FilePermanent::read(void *buffer, size_t size, size_t count)
{
    if (!fp)
        return 0;
    return fread(buffer, size, count, fp);
}

size_t FilePermanent::write(const void *buffer, size_t size, size_t count)
{
    if ( !( mode & (uint32_t)fsMode::Write ) ) {
        RE_ASSERT(false);
//        common->FatalError( "idFile_Permanent::Write: %s not opened in write mode", name.c_str() );
        return 0;
    }

    if (!fp)
        return 0;
    return fwrite(buffer, size, count, fp);
}

bool FilePermanent::eof()
{
    if (!fp || feof(fp))
        return true;
    return ((size_t)position()) >= length();
}

int64_t FilePermanent::position()
{
    if (!fp)
        return -1;
    return ftell(fp);
}

bool FilePermanent::seek(int64_t offset, int32_t origin)
{
    if (!fp)
        return false;
    return fseek(fp, offset, origin) == 0;
}

bool FilePermanent::rewind()
{
    if (canSeek())
    {
        std::rewind(fp);
        return true;
    }
    return false;
}

void FilePermanent::open()
{
    if (this->fp != NULL) {
        return;
    }

    // TODO: mode to char*
    FILE* _fp = fopen(fullPath.c_str(), "rw");

    RE_ASSERT(_fp);

    this->fp = _fp;
    this->checkLength();
}

void FilePermanent::close()
{
    if ( fp ) {
        fclose( fp );
    }

    fp = NULL;
}

void FilePermanent::checkLength()
{
    size_t		pos;
    size_t		end;

    pos = ftell( fp );
    fseek( fp, 0, SEEK_END );
    end = ftell( fp );
    fseek( fp, pos, SEEK_SET );

    this->fileSize = end;
}

}
