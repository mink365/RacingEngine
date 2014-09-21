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
    this->data = NULL;
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

    memset(data, 0, getDataByteSize());

    if (type == UniformType::MAT2) {
        this->initMatrixData(2);
    } else if (type == UniformType::MAT3) {
        this->initMatrixData(3);
    } else if (type == UniformType::MAT4) {
        this->initMatrixData(4);
    }
}

void Uniform::initMatrixData(int dimension)
{
    int offset = 0;
    for (int i = 0; i < nElements; ++i) {

        offset = dimension * dimension * i;

        for (int d = 0; d < dimension; ++d) {
            data[offset + d * dimension + d] = 1;
        }
    }
}

uint GetUniformSize(UniformType type) {
    switch (type) {
        case UniformType::BOOL: return 1;
        case UniformType::BVEC2: return 2;
        case UniformType::BVEC3: return 3;
        case UniformType::BVEC4: return 4;
        case UniformType::FLOAT: return 4;
        case UniformType::INT: return 4;
        case UniformType::IVEC2: return 4 * 2;
        case UniformType::IVEC3: return 4 * 3;
        case UniformType::IVEC4: return 4 * 4;
        case UniformType::MAT2: return 4 * 2 * 2;
        case UniformType::MAT3: return 4 * 3 * 3;
        case UniformType::MAT4: return 4 * 4 * 4;
        case UniformType::VEC2: return 4 * 2;
        case UniformType::VEC3: return 4 * 3;
        case UniformType::VEC4: return 4 * 4;
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

        memcpy(data, mat, dataSize);
    } else {
        memcpy(data, value, dataSize);
    }
}

int Uniform::getDataByteSize()
{
    return GetUniformSize(type) * nElements;
}

void Uniform::allocData()
{
    this->freeData();

    this->data = (float*)malloc(getDataByteSize());
}

void Uniform::freeData()
{
    if (data) {
        free(data);
        data = NULL;
    }
}

}
