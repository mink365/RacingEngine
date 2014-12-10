/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_BASEBITMAPFONTPARSER_H
#define RE_BASEBITMAPFONTPARSER_H

#include "PredefineType.h"

namespace re {

class BaseBitmapFontParser
{
public:
    BaseBitmapFontParser();

    FontPtr getFont();

    virtual void parseFile(FilePtr& file) = 0;

protected:
    FontPtr font;
    TexturePtr texture;
};

} // namespace re

#endif // RE_BASEBITMAPFONTPARSER_H
