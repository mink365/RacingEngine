/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef JSONATLASPARSER_H
#define JSONATLASPARSER_H

#include "BaseAtlasParser.h"

namespace re {

class JSONAtlasParser : public BaseAtlasParser
{
public:
    JSONAtlasParser();

protected:
    void parseAtlasFile(FilePtr& file);
};

}

#endif // JSONATLASPARSER_H
