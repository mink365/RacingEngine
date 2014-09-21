#ifndef UNIFORM_H
#define UNIFORM_H

#include <platform.h>

namespace re {

enum class UniformType {
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    INT,
    IVEC2,
    IVEC3,
    IVEC4,
    BOOL,
    BVEC2,
    BVEC3,
    BVEC4,
    MAT2,
    MAT3,
    MAT4,

    _COUNT
};

class Uniform
{
    friend class Shader;
public:
    Uniform();
    Uniform(const std::string name);
    ~Uniform();

    void init(const char *name, UniformType type, int location, int n);

    int getElementCount() const;
    void setElementCount(int size);

    int getLocation() const;
    UniformType getType() const;

    float* getData() const;
    void setData(float* value);

protected:
    void init();

    void initData();
    void initMatrixData(int dimension);

    int getDataByteSize();
    void allocData();
    void freeData();

private:
    std::string name;

    UniformType type;

    int location;

    /**
     * @brief nElements
     * Specifies the number of elements that are to be modified.
     * This should be 1 if the targeted uniform variable is not an array,
     * and 1 or more if it is an array.
     */
    int nElements;

    /**
     * @brief data
     * Specifies a pointer to an array of count values
     * that will be used to update the specified uniform variable.
     */
    float *data;
};

inline int Uniform::getElementCount() const
{
    return this->nElements;
}

inline int Uniform::getLocation() const
{
    return this->location;
}

inline UniformType Uniform::getType() const
{
    return this->type;
}

inline float *Uniform::getData() const
{
    return data;
}

} // namespace re

#endif // UNIFORM_H
