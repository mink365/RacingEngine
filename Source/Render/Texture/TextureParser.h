#ifndef RE_TEXTUREPARSER_H
#define RE_TEXTUREPARSER_H

#include "FileSystem/File.h"
#include "Base/Singleton.h"
#include "Base/NamedFactory.h"

namespace re {

class BaseAtlasParser;

typedef NamedFactory<BaseAtlasParser> AtlasParserFactory;

class TextureParser : public Singleton<TextureParser>
{
public:
    TextureParser();

    void addTextures(const std::string& dir, const std::string& extension);

    void parseSingle(FilePtr &file);

private:
    AtlasParserFactory factory;
};

} // namespace re

#endif // RE_TEXTUREPARSER_H
