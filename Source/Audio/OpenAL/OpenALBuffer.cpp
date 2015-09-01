/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "OpenALBuffer.h"
#include "platform.h"
#include "FileSystem/File.h"

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

ALenum __al_error_code = AL_NO_ERROR;

namespace re {

OpenALBuffer::OpenALBuffer(const std::string &path, ALuint& alBuffer)
    : _filePath(path), _alBuffer(alBuffer)
{

}

OpenALBuffer::~OpenALBuffer()
{

}

bool OpenALBuffer::loadWav(File* stream, ALuint buffer)
{
    RE_ASSERT(stream);

    unsigned char data[12];

    // Verify the wave fmt magic value meaning format.
    if (stream->read(data, 1, 8) != 8 || memcmp(data, "fmt ", 4) != 0 )
    {
        LogError("Failed to verify the magic value for the wave file format.");
        return false;
    }

    unsigned int section_size;
    section_size  = data[7]<<24;
    section_size |= data[6]<<16;
    section_size |= data[5]<<8;
    section_size |= data[4];

    // Check for a valid pcm format.
    if (stream->read(data, 1, 2) != 2 || data[1] != 0 || data[0] != 1)
    {
        LogError("Unsupported audio file format (must be a valid PCM format).");
        return false;
    }

    // Get the channel count (16-bit little-endian).
    int channels;
    if (stream->read(data, 1, 2) != 2)
    {
        LogError("Failed to read the wave file's channel count.");
        return false;
    }
    channels  = data[1]<<8;
    channels |= data[0];

    // Get the sample frequency (32-bit little-endian).
    ALuint frequency;
    if (stream->read(data, 1, 4) != 4)
    {
        LogError("Failed to read the wave file's sample frequency.");
        return false;
    }

    frequency  = data[3]<<24;
    frequency |= data[2]<<16;
    frequency |= data[1]<<8;
    frequency |= data[0];

    // The next 6 bytes hold the block size and bytes-per-second.
    // We don't need that info, so just read and ignore it.
    // We could use this later if we need to know the duration.
    if (stream->read(data, 1, 6) != 6)
    {
        LogError("Failed to read past the wave file's block size and bytes-per-second.");
        return false;
    }

    // Get the bit depth (16-bit little-endian).
    int bits;
    if (stream->read(data, 1, 2) != 2)
    {
        LogError("Failed to read the wave file's bit depth.");
        return false;
    }
    bits  = data[1]<<8;
    bits |= data[0];

    // Now convert the given channel count and bit depth into an OpenAL format.
    ALuint format = 0;
    if (bits == 8)
    {
        if (channels == 1)
            format = AL_FORMAT_MONO8;
        else if (channels == 2)
            format = AL_FORMAT_STEREO8;
    }
    else if (bits == 16)
    {
        if (channels == 1)
            format = AL_FORMAT_MONO16;
        else if (channels == 2)
            format = AL_FORMAT_STEREO16;
    }
    else
    {
        LogError("Incompatible wave file format: ({}, {}})", channels, bits);
        return false;
    }

    // Check against the size of the format header as there may be more data that we need to read.
    if (section_size > 16)
    {
        unsigned int length = section_size - 16;

        // Extension size is 2 bytes.
        if (stream->read(data, 1, length) != length)
        {
            LogError("Failed to read extension size from wave file.");
            return false;
        }
    }

    // Read in the rest of the file a chunk (section) at a time.
    while (true)
    {
        // Check if we are at the end of the file without reading the data.
        if (stream->eof())
        {
            LogError("Failed to load wave file; file appears to have no data.");
            return false;
        }

        // Read in the type of the next section of the file.
        if (stream->read(data, 1, 4) != 4)
        {
            LogError("Failed to read next section type from wave file.");
            return false;
        }

        // Data chunk.
        if (memcmp(data, "data", 4) == 0)
        {
            // Read how much data is remaining and buffer it up.
            unsigned int dataSize;
            if (stream->read(&dataSize, sizeof(int), 1) != 1)
            {
                LogError("Failed to read size of data section from wave file.");
                return false;
            }

            char* data = new char[dataSize];
            if (stream->read(data, sizeof(char), dataSize) != dataSize)
            {
                LogError("Failed to load wave file; file is missing data.");
                SAFE_DELETE_ARRAY(data);
                return false;
            }

            AL_CHECK( alBufferData(buffer, format, data, dataSize, frequency) );
            SAFE_DELETE_ARRAY(data);

            // We've read the data, so return now.
            return true;
        }
        // Other chunk - could be any of the following:
        // - Fact ("fact")
        // - Wave List ("wavl")
        // - Silent ("slnt")
        // - Cue ("cue ")
        // - Playlist ("plst")
        // - Associated Data List ("list")
        // - Label ("labl")
        // - Note ("note")
        // - Labeled Text ("ltxt")
        // - Sampler ("smpl")
        // - Instrument ("inst")
        else
        {
            // Store the name of the chunk so we can report errors informatively.
            char chunk[5] = { 0 };
            memcpy(chunk, data, 4);

            // Read the chunk size.
            if (stream->read(data, 1, 4) != 4)
            {
                LogError("Failed to read size of '{}' chunk from wave file.", chunk);
                return false;
            }

            section_size  = data[3]<<24;
            section_size |= data[2]<<16;
            section_size |= data[1]<<8;
            section_size |= data[0];

            // Seek past the chunk.
            if (stream->seek(section_size, SEEK_CUR) == false)
            {
                LogError("Failed to seek past '{}' chunk in wave file.", chunk);
                return false;
            }
        }
    }
    return false;
}

// Callbacks for loading an ogg file using Stream
static size_t readStream(void *ptr, size_t size, size_t nmemb, void *datasource)
{
    RE_ASSERT(datasource);
    File* stream = reinterpret_cast<File*>(datasource);
    return stream->read(ptr, size, nmemb);
}

static int seekStream(void *datasource, ogg_int64_t offset, int whence)
{
    RE_ASSERT(datasource);
    File* stream = reinterpret_cast<File*>(datasource);
    return !stream->seek(offset, whence);
}

static int closeStream(void *datasource)
{
    RE_ASSERT(datasource);
    File* stream = reinterpret_cast<File*>(datasource);
    stream->close();
    return 0;
}

static long tellStream(void *datasource)
{
    RE_ASSERT(datasource);
    File* stream = reinterpret_cast<File*>(datasource);
    return stream->position();
}

bool OpenALBuffer::loadOgg(File* stream, ALuint buffer)
{
    RE_ASSERT(stream);

    OggVorbis_File ogg_file;
    vorbis_info* info;
    ALenum format;
    long result;
    int section;
    long size = 0;

    stream->rewind();

    ov_callbacks callbacks;
    callbacks.read_func = readStream;
    callbacks.seek_func = seekStream;
    callbacks.close_func = closeStream;
    callbacks.tell_func = tellStream;

    if ((result = ov_open_callbacks(stream, &ogg_file, NULL, 0, callbacks)) < 0)
    {
        LogError("Failed to open ogg file.");
        return false;
    }

    info = ov_info(&ogg_file, -1);
    RE_ASSERT(info);
    if (info->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;

    // size = #samples * #channels * 2 (for 16 bit).
    long data_size = ov_pcm_total(&ogg_file, -1) * info->channels * 2;
    char* data = new char[data_size];

    while (size < data_size)
    {
        result = ov_read(&ogg_file, data + size, data_size - size, 0, 2, 1, &section);
        if (result > 0)
        {
            size += result;
        }
        else if (result < 0)
        {
            SAFE_DELETE_ARRAY(data);
            LogError("Failed to read ogg file; file is missing data.");
            return false;
        }
        else
        {
            break;
        }
    }

    if (size == 0)
    {
        SAFE_DELETE_ARRAY(data);
        LogError("Filed to read ogg file; unable to read any data.");
        return false;
    }

    AL_CHECK( alBufferData(buffer, format, data, data_size, info->rate) );

    SAFE_DELETE_ARRAY(data);
    ov_clear(&ogg_file);

    return true;
}

} // namespace re

