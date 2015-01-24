#include "OpenALAudioFactory.h"

#include "FileSystem/FileSystem.h"

namespace re {

OpenALAudioFactory::OpenALAudioFactory()
{

}

OpenALAudioFactory::~OpenALAudioFactory()
{

}

OpenALBuffer::ptr OpenALAudioFactory::CreateBuffer(const std::string &path)
{
    ALuint alBuffer;
    alGenBuffers(1, &alBuffer);

    auto file = FileSystem::getInstance().getFile(path);

    RE_ASSERT(file != nullptr);

    file->open();
    OpenALBuffer::loadOgg(file.get(), alBuffer);

    OpenALBuffer::ptr buffer = std::make_shared<OpenALBuffer>(path, alBuffer);

    return buffer;
}

} // namespace re

