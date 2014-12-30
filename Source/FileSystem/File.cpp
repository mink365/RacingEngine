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

    this->read(buf->getData(), this->length());

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

size_t FilePermanent::read(void *buffer, size_t len)
{
    size_t		block, remaining;
    size_t		read;
    byte *	buf;
    size_t		tries;

    if ( !( mode & (uint32_t)fsMode::Read ) ) {
        RE_ASSERT(false);
//        common->FatalError( "idFile_Permanent::Read: %s not opened in read mode", name.c_str() );
        return 0;
    }

    if ( !fp ) {
        return 0;
    }

    buf = (byte *)buffer;

    remaining = len;
    tries = 0;
    while( remaining ) {
        block = remaining;
        read = fread( buf, 1, block, fp );
        if ( read == 0 ) {
            // we might have been trying to read from a CD, which
            // sometimes returns a 0 read on windows
            if ( !tries ) {
                tries = 1;
            } else {
                // TODO:
//                fileSystem->AddToReadCount( len - remaining );
                return len-remaining;
            }
        }

        if ( read == -1 ) {
            RE_ASSERT(false);
//            common->FatalError( "idFile_Permanent::Read: -1 bytes read from %s", name.c_str() );
        }

        remaining -= read;
        buf += read;
    }
//    fileSystem->AddToReadCount( len );
    return len;
}

size_t FilePermanent::write(const void *buffer, size_t len)
{
    int		block, remaining;
    int		written;
    byte *	buf;
    int		tries;

    if ( !( mode & (uint32_t)fsMode::Write ) ) {
        RE_ASSERT(false);
//        common->FatalError( "idFile_Permanent::Write: %s not opened in write mode", name.c_str() );
        return 0;
    }

    if ( !fp ) {
        return 0;
    }

    buf = (byte *)buffer;

    remaining = len;
    tries = 0;
    while( remaining ) {
        block = remaining;
        written = fwrite( buf, 1, block, fp );
        if ( written == 0 ) {
            if ( !tries ) {
                tries = 1;
            }
            else {
                RE_ASSERT(false);
//                common->Printf( "idFile_Permanent::Write: 0 bytes written to %s\n", name.c_str() );
                return 0;
            }
        }

        if ( written == -1 ) {
            RE_ASSERT(false);
//            common->Printf( "idFile_Permanent::Write: -1 bytes written to %s\n", name.c_str() );
            return 0;
        }

        remaining -= written;
        buf += written;
        fileSize += written;
    }
//    if ( handleSync ) {
//        fflush( o );
//    }
    return len;
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
