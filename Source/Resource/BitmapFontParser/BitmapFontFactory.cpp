/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#include "BitmapFontFactory.h"

#include "AngleFontParser.h"

namespace re {

namespace BitmapFontType {
    std::string Angle = "Angle";
}

BitmapFontFactory::BitmapFontFactory()
{
    factory.registerCreateFunc(BitmapFontType::Angle, [](){
        return Create<AngleFontParser>();
    });
}

FontPtr BitmapFontFactory::CreateFont(FilePtr &font, const std::string &fontType)
{
    auto parser = factory.createInstance(fontType);

    parser->parseFile(font);

    return parser->getFont();
}

} // namespace re
