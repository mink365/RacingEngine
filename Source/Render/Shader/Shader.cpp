#include "Shader.h"

namespace re {

Shader::Shader()
{
    this->init();
}

Shader::Shader(const char *vertSource, const char *fragSource)
{
    this->init();

    this->vertexSource = vertSource;
    this->fragmentSource = fragSource;
}

GPU_ID Shader::getVertexShader() const
{
    return vertexShader;
}

void Shader::setVertexShader(const GPU_ID &value)
{
    vertexShader = value;
}

GPU_ID Shader::getFragmentShader() const
{
    return fragmentShader;
}

void Shader::setFragmentShader(const GPU_ID &value)
{
    fragmentShader = value;
}

GPU_ID Shader::getProgram() const
{
    return program;
}

void Shader::setProgram(const GPU_ID &value)
{
    program = value;
}

bool Shader::getIsCompled() const
{
    return isCompled;
}

void Shader::setIsCompled(bool value)
{
    isCompled = value;
}

std::string Shader::getFragmentSource() const
{
    return fragmentSource;
}

void Shader::setFragmentSource(const std::string &value)
{
    fragmentSource = value;
}

std::string Shader::getVertexSource() const
{
    return vertexSource;
}

void Shader::setVertexSource(const std::string &value)
{
    vertexSource = value;
}

std::vector<Attribute *> Shader::getAttributes() const
{
    return attributes;
}

Attribute *Shader::getAttribute(const std::string name)
{
    std::vector<Attribute *>::iterator iter;
    for (iter = this->attributes.begin(); iter != this->attributes.end(); ++iter) {
        Attribute* attr = *iter;

        if (attr->name == name) {
            return attr;
        }
    }

    return nullptr;
}

void Shader::addAttribute(Attribute *value)
{
    this->attributes.push_back(value);
}

std::vector<Uniform *> Shader::getUniforms() const
{
    return uniforms;
}

Uniform *Shader::getUniform(const std::string name)
{
    std::vector<Uniform *>::iterator iter;
    for (iter = this->uniforms.begin(); iter != this->uniforms.end(); ++iter) {
        Uniform* uniform = *iter;

        if (uniform->name == name) {
            return uniform;
        }
    }

    return NULL;
}

void Shader::addUniform(Uniform *value)
{
    this->uniforms.push_back(value);
}

void Shader::init()
{
    this->isCompled = false;
    this->program = -1;
    this->vertexShader = -1;
    this->vertexSource = -1;
}

}
