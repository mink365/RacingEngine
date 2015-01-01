/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_OPENALBUFFER_H
#define RE_OPENALBUFFER_H

#include "OpenALBase.h"
#include "Base/Shared.h"

namespace re {

class File;

class OpenALBuffer : public Shared<OpenALBuffer>
{
public:
    OpenALBuffer();
    ~OpenALBuffer();

public:
    static bool loadWav(File* stream, ALuint buffer);

    static bool loadOgg(File* stream, ALuint buffer);

private:
    std::string _filePath;
    ALuint _alBuffer;
};

} // namespace re

#endif // RE_OPENALBUFFER_H
