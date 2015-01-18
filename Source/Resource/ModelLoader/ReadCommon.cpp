/*
 * ReadCommon.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jk
 */

#include "ReadCommon.h"
#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include "Math/Matrix.h"

#include <iostream>
#include <fstream>

namespace re {

ReadCommon::ReadCommon() {

}

ReadCommon::~ReadCommon() {

}

bool ReadCommon::ReadBoolean(std::istream *stream) {
    Int v;

    stream->read((char*)&v, 4);

    return (v != 0);
}

Int ReadCommon::ReadInt(std::istream *stream) {
  Int v = 0;

  stream->read((char*)&v, 4);

  return v;
}

Long ReadCommon::ReadLong(std::istream *stream) {
    Long v = 0;

    stream->read((char*)&v, 8);

    return v;
}

float ReadCommon::ReadFloat(std::istream *stream) {
    float v = 0;

    stream->read((char*)&v, 4);

    return v;
}

std::string ReadCommon::ReadString(std::istream *stream) {
    Int count = ReadInt(stream);

    LOG_D(" char count: %d", count);

    char v[count + 1];

    stream->read(v, count);

    // 给C风格的字符串一个明确的结束字符
    v[count] = '\0';

    LOG_D(" read it: %s", v);

    return v;
}

Vec3 ReadCommon::ReadVec3(std::istream *stream) {
    float v[3];

    stream->read((char*)v, 3 * 4);

    return re::Vec3(v[0], v[1], v[2]);
}

Mat4 ReadCommon::ReadMat4(istream *st)
{
    Vec3 vectorT = this->ReadVec3(st);
    Vec3 vectorR = this->ReadVec3(st);
    Vec3 vectorS = this->ReadVec3(st);

    vectorR *= DEG_TO_RAD;

    Mat4 mat;

    mat.fromRTS(vectorR.toQuat(), vectorT, vectorS);

    return mat;
}

}
