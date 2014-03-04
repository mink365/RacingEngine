/*
 * ReadCommon.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "ReadCommon.h"
#include "math/Vector.h"

#include <iostream>
#include <fstream>

ReadCommon::ReadCommon() {
	// TODO Auto-generated constructor stub

}

ReadCommon::~ReadCommon() {
	// TODO Auto-generated destructor stub
}

bool ReadCommon::ReadBoolean(std::istream *stream) {
    int v;

    stream->read((char*)&v, 4);

    return (v != 0);
}

int ReadCommon::ReadInt(std::istream *stream) {
  int v = 0;

  stream->read((char*)&v, 4);

  return v;
}

long ReadCommon::ReadLong(std::istream *stream) {
    long v = 0;

    stream->read((char*)&v, 8);

    return v;
}

float ReadCommon::ReadFloat(std::istream *stream) {
    float v = 0;

    stream->read((char*)&v, 4);

    return v;
}

std::string ReadCommon::ReadString(std::istream *stream) {
    int count = ReadInt(stream);

    std::cout << " char count: " << count << std::endl;

    char v[count + 1];

    stream->read(v, count);

    // 给C风格的字符串一个明确的结束字符
    v[count] = '\0';

    std::cout << " read it " << v << std::endl;

    return v;
}

reVec3 ReadCommon::ReadVec3(std::istream *stream) {
    float v[3];

    stream->read((char*)v, 3 * 4);

    return reVec3(v[0], v[1], v[2]);
}
