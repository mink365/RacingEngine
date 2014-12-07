#include "File.h"

#include "FileSystem.h"

typedef unsigned char			byte;		// 8 bits

namespace re {

File::File()
{
    this->fileSize = -1;
}

File::~File()
{

}

int File::length() const
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
    FilePtr p = this->shared_from_this();

    FileSystem::getInstance().openFile(p, fsMode::Read);

    ByteBufferPtr buf = std::make_shared<ByteBuffer>(this->length());

    this->read(buf->getData(), this->length());

    return buf;
}

FilePermanent::FilePermanent()
{
    type = FileType::Permanent;

    name = "invalid";
    fullPath = "invalid";
    fp = NULL;
    mode = 0;
    fileSize = 0;
}

FilePermanent::~FilePermanent()
{
    if ( fp ) {
        fclose( fp );
    }
}

const std::string &FilePermanent::getName() const
{
    return this->name;
}

const std::string &FilePermanent::getFullPath() const
{
    return this->fullPath;
}

int FilePermanent::read(void *buffer, int len)
{
    int		block, remaining;
    int		read;
    byte *	buf;
    int		tries;

    if ( !( mode & ( 1 << (int)fsMode::Read ) ) ) {
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
//            common->FatalError( "idFile_Permanent::Read: -1 bytes read from %s", name.c_str() );
        }

        remaining -= read;
        buf += read;
    }
//    fileSystem->AddToReadCount( len );
    return len;
}

int FilePermanent::write(const void *buffer, int len)
{
    int		block, remaining;
    int		written;
    byte *	buf;
    int		tries;

    if ( !( mode & ( 1 << (int)fsMode::Write ) ) ) {
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
//                common->Printf( "idFile_Permanent::Write: 0 bytes written to %s\n", name.c_str() );
                return 0;
            }
        }

        if ( written == -1 ) {
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

void FilePermanent::checkLength()
{
    int		pos;
    int		end;

    pos = ftell( fp );
    fseek( fp, 0, SEEK_END );
    end = ftell( fp );
    fseek( fp, pos, SEEK_SET );

    this->fileSize = end;
}

}
