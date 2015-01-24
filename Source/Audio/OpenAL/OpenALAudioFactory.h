#ifndef RE_OPENALAUDIOFACTORY_H
#define RE_OPENALAUDIOFACTORY_H

#include "Base/Singleton.h"
#include "OpenALBuffer.h"

namespace re {

class OpenALAudioFactory : public Singleton<OpenALAudioFactory>
{
public:
    OpenALAudioFactory();
    ~OpenALAudioFactory();

    OpenALBuffer::ptr CreateBuffer(const std::string& path);
};

} // namespace re

#endif // RE_OPENALAUDIOFACTORY_H
