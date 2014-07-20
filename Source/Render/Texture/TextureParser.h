#ifndef RE_TEXTUREPARSER_H
#define RE_TEXTUREPARSER_H

#include "FileSystem/File.h"
#include "Base/Singleton.h"

namespace re {

class TextureParser : public Singleton<TextureParser>
{
public:
    TextureParser();

    void addTextures(const std::string& dir, const std::string& extension);

    void parseSingle(FilePtr &file);
};

} // namespace re

#endif // RE_TEXTUREPARSER_H
