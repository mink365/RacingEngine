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
    this->type = UNIFORM_FLOAT;
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

    if (type == UNIFORM_MAT2) {
        this->initMatrixData(2);
    } else if (type == UNIFORM_MAT3) {
        this->initMatrixData(3);
    } else if (type == UNIFORM_MAT4) {
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
        case UNIFORM_BOOL: return 1;
        case UNIFORM_BVEC2: return 2;
        case UNIFORM_BVEC3: return 3;
        case UNIFORM_BVEC4: return 4;
        case UNIFORM_FLOAT: return 4;
        case UNIFORM_INT: return 4;
        case UNIFORM_IVEC2: return 4 * 2;
        case UNIFORM_IVEC3: return 4 * 3;
        case UNIFORM_IVEC4: return 4 * 4;
        case UNIFORM_MAT2: return 4 * 2 * 2;
        case UNIFORM_MAT3: return 4 * 3 * 3;
        case UNIFORM_MAT4: return 4 * 4 * 4;
        case UNIFORM_VEC2: return 4 * 2;
        case UNIFORM_VEC3: return 4 * 3;
        case UNIFORM_VEC4: return 4 * 4;
        case UNIFORM_TYPE_COUNT: return 0;
    }

    return 0;
}

void Uniform::setData(float *value)
{
    int dataSize = getDataByteSize();

    if (type == UNIFORM_MAT2 || type == UNIFORM_MAT4 || type == UNIFORM_MAT3) {
        // row-major to column-major

        int cal = 2 + type - UNIFORM_MAT2;
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
