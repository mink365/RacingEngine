/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef RE_ANGLEFONTPARSER_H
#define RE_ANGLEFONTPARSER_H

#include "BaseBitmapFontParser.h"
#include "PredefineType.h"

namespace re {

class AngleFontParser : public BaseBitmapFontParser
{
public:
    AngleFontParser();

    void parseFile(FilePtr& file);

protected:
    void createFont(FilePtr &file);

private:
    int size;
    std::string family;
    std::string style;

    int width;
    int height;

    // only support one page
    std::string imageName;
};

} // namespace re

#endif // RE_ANGLEFONTPARSER_H
