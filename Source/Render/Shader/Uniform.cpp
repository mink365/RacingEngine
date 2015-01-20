#include "Uniform.h"

namespace re {

Uniform::Uniform()
{
    this->init();
}

Uniform::Uniform(const std::string name)
{
    this->init();

    this->name = name;
}

Uniform::~Uniform()
{
    this->freeData();
}

void Uniform::init()
{
    this->location = -1;
    this->type = UniformType::FLOAT;
    this->nElements = 0;
    this->floatData = NULL;
}

void Uniform::init(const char* name, UniformType type, int location, int n)
{
    this->name = name;
    this->type = type;
    this->location = location;
    this->nElements = n;

    this->initData();
}

void Uniform::setElementCount(int size)
{
    this->nElements = size;

    this->initData();
}

void Uniform::initData()
{
    this->allocData();

    memset(floatData, 0, getDataByteSize());
}

uint GetUniformSize(UniformType type) {
    switch (type) {
        case UniformType::BOOL:  return 1;
        case UniformType::BVEC2: return 2;
        case UniformType::BVEC3: return 3;
        case UniformType::BVEC4: return 4;
        case UniformType::FLOAT: return 4;
        case UniformType::INT:   return 4;
        case UniformType::IVEC2: return 4 * 2;
        case UniformType::IVEC3: return 4 * 3;
        case UniformType::IVEC4: return 4 * 4;
        case UniformType::MAT2:  return 4 * 2 * 2;
        case UniformType::MAT3:  return 4 * 3 * 3;
        case UniformType::MAT4:  return 4 * 4 * 4;
        case UniformType::VEC2:  return 4 * 2;
        case UniformType::VEC3:  return 4 * 3;
        case UniformType::VEC4:  return 4 * 4;
        case UniformType::SAMPLER_1D: return 4;
        case UniformType::SAMPLER_2D: return 4;
        case UniformType::SAMPLER_3D: return 4;
        case UniformType::SAMPLER_CUBE: return 4;
        case UniformType::SAMPLER_1D_ARRAY: return 4;
        case UniformType::SAMPLER_2D_ARRAY: return 4;
        case UniformType::SAMPLER_3D_ARRAY: return 4;
        case UniformType::SAMPLER_CUBE_ARRAY: return 4;
        case UniformType::_COUNT: return 0;
    }

    return 0;
}

void Uniform::setData(float *value)
{
    int dataSize = getDataByteSize();

    if (type == UniformType::MAT2 || type == UniformType::MAT4 || type == UniformType::MAT3) {
        // row-major to column-major

        int cal = 2 + (int)type - (int)UniformType::MAT2;
        int size = cal * cal;
        float mat[dataSize];

        for (int n = 0; n < nElements; ++n) {
            int offset = n * size;

            for (int i = 0; i < cal; ++i) {
                for (int j = 0; j < cal; ++j) {
                    mat[i * cal + j + offset] = value[j * cal + i + offset];
                }
            }
        }

        memcpy(floatData, mat, dataSize);
    } else {
        memcpy(floatData, value, dataSize);
    }
}

void Uniform::setValue_(void *value)
{
    int dataSize = getDataByteSize();

    memcpy(floatData, value, dataSize);
}

int Uniform::getDataByteSize()
{
    return GetUniformSize(type) * nElements;
}

void Uniform::allocData()
{
    this->freeData();

    this->floatData = (float*)malloc(getDataByteSize());
}

void Uniform::freeData()
{
    if (floatData) {
        free(floatData);
        floatData = NULL;
    }
}

}
