/*
 * ReadCommon.h
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#ifndef READCOMMON_H_
#define READCOMMON_H_

#include <fstream>

namespace re {

class Vec3;

class ReadCommon {
public:
	ReadCommon();

    bool ReadBoolean(std::istream *st);
    int ReadInt(std::istream *st);
    long ReadLong(std::istream *st);
    float ReadFloat(std::istream *st);
    std::string ReadString(std::istream *st);

    Vec3 ReadVec3(std::istream *st);

	virtual ~ReadCommon();
};

} // namespace re

#endif /* READCOMMON_H_ */
