#ifndef SHADER_H
#define SHADER_H

#include "platform.h"
#include <vector>
#include "Attribute.h"
#include "Uniform.h"
#include "Sampler.h"
#include "Base/Named.h"
#include "Base/Shared.h"

namespace re {

class Shader : public Named, public Shared<Shader>
{
    friend class ShaderUtil;
public:
    Shader();
    Shader(const char *vertSource, const char *fragSource);

    GPU_ID getVertexShader() const;
    void setVertexShader(const GPU_ID &value);

    GPU_ID getFragmentShader() const;
    void setFragmentShader(const GPU_ID &value);

    GPU_ID getProgram() const;
    void setProgram(const GPU_ID &value);

    bool getIsCompled() const;
    void setIsCompled(bool value);

    std::string getVertexSource() const;
    void setVertexSource(const std::string &value);

    std::string getFragmentSource() const;
    void setFragmentSource(const std::string &value);

    std::vector<Attribute *> getAttributes() const;
    Attribute *getAttribute(const std::string name);
    void addAttribute(Attribute *value);

    std::vector<Uniform *> getUniforms() const;
    Uniform *getUniform(const std::string name);
    void addUniform(Uniform *value);

protected:
    void init();

private:
    std::string vertexSource;
    std::string fragmentSource;

    GPU_ID program;
    GPU_ID vertexShader;
    GPU_ID fragmentShader;

    std::vector<Uniform *> uniforms;
    std::vector<Attribute *> attributes;

    bool isCompled;
};

} // namespace re

#endif // SHADER_H
