#ifndef SHADERUTIL_H
#define SHADERUTIL_H

#include "Base/Singleton.h"

#include "Attribute.h"
#include "Uniform.h"
#include "Shader.h"

namespace re {

class ShaderUtil : public Singleton<ShaderUtil>
{
    friend class Singleton;

public:
    void compileShader(Shader *shader);
    /**
     * @brief bindShader
     * @param shader
     * 绑定attri和uniform，必须在bindVBO之后执行
     */
    void bindShader(Shader *shader);
private:
    ShaderUtil();
    void applyAttributeToHardware(Attribute *attribute);
    void applyUniformToHardware(Uniform *uniform);

    void fetchAttributes(Shader *shader);
    void fetchUniforms(Shader *shader);

    uint loadShader(const char *source, uint type);
    void linkPrograme(Shader *shader);
};

} // namespace re

#endif // SHADERUTIL_H
