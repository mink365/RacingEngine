#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>

namespace re {

enum AttributeFormat {
    ATTR_FORMAT_FLOAT = 0,
    ATTR_FORMAT_HALF  = 1,
    ATTR_FORMAT_UBYTE = 2
};

class Attribute
{
    friend class ShaderUtil;
    friend class Shader;

public:
    Attribute();
    Attribute(const std::string name);

    void setValue(int location, int size, AttributeFormat type, int stribe, int offset);

    int getStride() const;
    void setStride(int value);

    AttributeFormat getType() const;
    void setType(const AttributeFormat value);

    int getOffset() const;
    void setOffset(int value);

    int getSize() const;
    void setSize(int value);

    int getLocation() const;

    bool getNormalized() const;
    void setNormalized(bool value);

protected:
    void init();

private:
    std::string name;

    int location;

    /**
     * @brief size
     * Specifies the number of components per generic vertex attribute.
     * Must be 1, 2, 3, 4. Additionally, the symbolic constant GL_BGRA is accepted by glVertexAttribPointer.
     * The initial value is 4.
     *
     * size 依赖于数据类型，比如，type为Float,则普通的顶点的size为3,三个float表示一个顶点
     */
    int size;

    /**
     * @brief type
     * Specifies the data type of each component in the array. The symbolic constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,
     * GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT are accepted by glVertexAttribPointer and glVertexAttribIPointer.
     * Additionally GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, and GL_UNSIGNED_INT_2_10_10_10_REV
     * are accepted by glVertexAttribPointer. GL_DOUBLE is also accepted by glVertexAttribLPointer and is the only token accepted by the type parameter for that function. The initial value is GL_FLOAT.
     */
    AttributeFormat type;

    /**
     * @brief normalized
     * For glVertexAttribPointer, specifies whether fixed-point data values should be normalized (GL_TRUE)
     * or converted directly as fixed-point values (GL_FALSE) when they are accessed.
     */
    bool normalized;

    /**
     * @brief stride
     * Specifies the byte offset between consecutive generic vertex attributes. If stride is 0,
     * the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
     */
    int stride;

    /**
     * @brief offset
     * Specifies a offset of the first component of the first generic vertex attribute in the array in the
     * data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
     */
    int offset;
};

} // namespace re

#endif // ATTRIBUTE_H
