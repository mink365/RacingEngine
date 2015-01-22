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
    SAMPLER_1D,
    SAMPLER_2D,
    SAMPLER_3D,
    SAMPLER_CUBE,
    SAMPLER_1D_ARRAY,
    SAMPLER_2D_ARRAY,
    SAMPLER_3D_ARRAY,
    SAMPLER_CUBE_ARRAY,

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

    template<typename T>
    const T* getData() const;

    void setData(float* value);
    void setData(int32_t* value);
    void setData(int16_t* value);
    void setData(int8_t*  value);

private:
    void setValue_(void* value);

protected:
    void init();

    void initData();

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
    byte* data;
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

inline void Uniform::setData(int32_t *value)
{
    setValue_(value);
}

inline void Uniform::setData(int16_t *value)
{
    setValue_(value);
}

inline void Uniform::setData(int8_t *value)
{
    setValue_(value);
}

template <typename T>
inline const T *Uniform::getData() const
{
    return (T*)(data);
}

} // namespace re

#endif // UNIFORM_H
