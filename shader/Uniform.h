#ifndef UNIFORM_H
#define UNIFORM_H

#include <platform.h>

namespace re {

enum UniformType {
    UNIFORM_FLOAT,
    UNIFORM_VEC2,
    UNIFORM_VEC3,
    UNIFORM_VEC4,
    UNIFORM_INT,
    UNIFORM_IVEC2,
    UNIFORM_IVEC3,
    UNIFORM_IVEC4,
    UNIFORM_BOOL,
    UNIFORM_BVEC2,
    UNIFORM_BVEC3,
    UNIFORM_BVEC4,
    UNIFORM_MAT2,
    UNIFORM_MAT3,
    UNIFORM_MAT4,

    UNIFORM_TYPE_COUNT
};

class Uniform
{
    friend class Shader;
    friend class ShaderUtil;
public:
    Uniform();
    Uniform(const std::string name);

    float* getData() const;
    void setData(float* value);

protected:
    void init();

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

} // namespace re

#endif // UNIFORM_H
