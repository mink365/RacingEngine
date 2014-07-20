/*
 * ReadCommon.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef READCOMMON_H_
#define READCOMMON_H_

#include <fstream>
#include "platform.h"

namespace re {

class Vec3;
class Mat4;

class ReadCommon {
public:
	ReadCommon();

    bool ReadBoolean(std::istream *st);
    Int ReadInt(std::istream *st);
    Long ReadLong(std::istream *st);
    float ReadFloat(std::istream *st);
    std::string ReadString(std::istream *st);

    Vec3 ReadVec3(std::istream *st);
    Mat4 ReadMat4(std::istream *st);

	virtual ~ReadCommon();
};

} // namespace re

#endif /* READCOMMON_H_ */
