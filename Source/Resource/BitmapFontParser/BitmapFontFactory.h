/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_BITMAPFONTFACTORY_H
#define RE_BITMAPFONTFACTORY_H

#include "Base/Singleton.h"
#include "Base/NamedFactory.h"
#include "BaseBitmapFontParser.h"

namespace re {

namespace BitmapFontType {
    extern std::string Angle;
}

typedef NamedFactory<BaseBitmapFontParser> BitmapFontParserFactory;

class BitmapFontFactory : public Singleton<BitmapFontFactory>
{
public:
    BitmapFontFactory();

    FontPtr CreateFont(FilePtr& font, const std::string &fontType);

public:
    BitmapFontParserFactory factory;
};

} // namespace re

#endif // RE_BITMAPFONTFACTORY_H
