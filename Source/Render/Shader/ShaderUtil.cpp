#include "ShaderUtil.h"

#include "platform.h"

#include "opengl.h"

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "Math/Matrix.h"

namespace re {

const static GLenum glTypes[] = {
    GL_FLOAT,
    0, // No half float support
    GL_UNSIGNED_BYTE,
};

#ifndef APIENTRY
#define APIENTRY
#endif

typedef GLvoid (APIENTRY *UNIFORM_FUNC)(GLint location, GLsizei count, const void *value);
typedef GLvoid (APIENTRY *UNIFORM_MAT_FUNC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void *uniformFuncs[(int)UniformType::_COUNT];

// we just support 16 texture unit now
int32_t TextureUnits[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

AttributeFormat getAttributeType(GLenum type) {
    switch(type) {
        case GL_FLOAT: return AttributeFormat::FLOAT;
        case GL_UNSIGNED_BYTE: return AttributeFormat::UBYTE;
    }

    return (AttributeFormat) - 1;
}

UniformType getUniformType(GLenum type) {
    switch (type){
        case GL_FLOAT:      return UniformType::FLOAT;
        case GL_FLOAT_VEC2: return UniformType::VEC2;
        case GL_FLOAT_VEC3: return UniformType::VEC3;
        case GL_FLOAT_VEC4: return UniformType::VEC4;
        case GL_INT:        return UniformType::INT;
        case GL_INT_VEC2:   return UniformType::IVEC2;
        case GL_INT_VEC3:   return UniformType::IVEC3;
        case GL_INT_VEC4:   return UniformType::IVEC4;
        case GL_BOOL:       return UniformType::BOOL;
        case GL_BOOL_VEC2:  return UniformType::BVEC2;
        case GL_BOOL_VEC3:  return UniformType::BVEC3;
        case GL_BOOL_VEC4:  return UniformType::BVEC4;
        case GL_FLOAT_MAT2: return UniformType::MAT2;
        case GL_FLOAT_MAT3: return UniformType::MAT3;
        case GL_FLOAT_MAT4: return UniformType::MAT4;
        case GL_SAMPLER_1D: return UniformType::SAMPLER_1D;
        case GL_SAMPLER_2D: return UniformType::SAMPLER_2D;
        case GL_SAMPLER_3D: return UniformType::SAMPLER_3D;
        case GL_SAMPLER_CUBE:     return UniformType::SAMPLER_CUBE;
#if !GLES
        case GL_SAMPLER_1D_ARRAY: return UniformType::SAMPLER_1D_ARRAY;
        case GL_SAMPLER_2D_ARRAY: return UniformType::SAMPLER_2D_ARRAY;
#endif

    }

    return (UniformType) -1;
}

ShaderUtil::ShaderUtil()
{
    uniformFuncs[(int)UniformType::FLOAT] = (void *) glUniform1fv;
    uniformFuncs[(int)UniformType::VEC2]  = (void *) glUniform2fv;
    uniformFuncs[(int)UniformType::VEC3]  = (void *) glUniform3fv;
    uniformFuncs[(int)UniformType::VEC4]  = (void *) glUniform4fv;
    uniformFuncs[(int)UniformType::INT]   = (void *) glUniform1iv;
    uniformFuncs[(int)UniformType::IVEC2] = (void *) glUniform2iv;
    uniformFuncs[(int)UniformType::IVEC3] = (void *) glUniform3iv;
    uniformFuncs[(int)UniformType::IVEC4] = (void *) glUniform4iv;
    uniformFuncs[(int)UniformType::BOOL]  = (void *) glUniform1iv;
    uniformFuncs[(int)UniformType::BVEC2] = (void *) glUniform2iv;
    uniformFuncs[(int)UniformType::BVEC3] = (void *) glUniform3iv;
    uniformFuncs[(int)UniformType::BVEC4] = (void *) glUniform4iv;
    uniformFuncs[(int)UniformType::MAT2]  = (void *) glUniformMatrix2fv;
    uniformFuncs[(int)UniformType::MAT3]  = (void *) glUniformMatrix3fv;
    uniformFuncs[(int)UniformType::MAT4]  = (void *) glUniformMatrix4fv;
    uniformFuncs[(int)UniformType::SAMPLER_1D] = (void *) glUniform1iv;
    uniformFuncs[(int)UniformType::SAMPLER_2D] = (void *) glUniform1iv;
    uniformFuncs[(int)UniformType::SAMPLER_3D] = (void *) glUniform1iv;
    uniformFuncs[(int)UniformType::SAMPLER_CUBE]     = (void *) glUniform1iv;
    uniformFuncs[(int)UniformType::SAMPLER_1D_ARRAY] = (void *) glUniform1iv;
    uniformFuncs[(int)UniformType::SAMPLER_2D_ARRAY] = (void *) glUniform1iv;

}

typedef void (*GLInfoFunction) (GLuint program, GLenum pname, GLint* params);
typedef void (*GLLogFunction) (GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog);
const char* logForOpenGLObject(GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc)
{
    GLint logLength = 0, charsWritten = 0;

    infoFunc(object, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength < 1)
        return 0;

    char *logBytes = (char*)malloc(logLength);
    logFunc(object, logLength, &charsWritten, logBytes);

    std::string* log = new std::string(logBytes);

    free(logBytes);
    return log->c_str();
}

void ShaderUtil::compileShader(Shader *shader)
{
    uint vertexShaderID = this->loadShader(shader->getVertexSource().c_str(), GL_VERTEX_SHADER);
    uint fragmentShaderID = this->loadShader(shader->getFragmentSource().c_str(), GL_FRAGMENT_SHADER);

    if (vertexShaderID != 0 && fragmentShaderID != 0) {
        shader->setVertexShader(vertexShaderID);
        shader->setFragmentShader(fragmentShaderID);
    } else {
        shader->setIsCompled(false);
        return;
    }

    this->linkPrograme(shader);

    if (shader->getIsCompled()) {
        this->fetchAttributes(shader);
        this->fetchUniforms(shader);
    }
}

void ShaderUtil::bindShader(Shader *shader)
{
    glUseProgram(shader->getProgram());

    for (Attribute* attribute : shader->getAttributes()) {
        this->applyAttributeToHardware(attribute);
    }

    for (Uniform* uniform : shader->getUniforms()) {
        this->applyUniformToHardware(uniform);
    }
}

void ShaderUtil::applyAttributeToHardware(Attribute *attr)
{
    glEnableVertexAttribArray(attr->location);
    glVertexAttribPointer(attr->location, attr->size, glTypes[(int)(attr->type)], attr->normalized, attr->stride, (GLvoid *)attr->offset);
}

void ShaderUtil::applyUniformToHardware(Uniform *uniform)
{
    if (uniform->getType() >= UniformType::MAT2 && uniform->getType() <= UniformType::MAT4) {
        ((UNIFORM_MAT_FUNC) uniformFuncs[(int)uniform->getType()])(uniform->getLocation(), uniform->getElementCount(), GL_FALSE, uniform->getData<float>());
    } else {
        ((UNIFORM_FUNC) uniformFuncs[(int)uniform->getType()])(uniform->getLocation(), uniform->getElementCount(), uniform->getData<float>());
    }
}

void ShaderUtil::fetchAttributes(Shader *shader)
{
    GLint attributeCount = 0, maxLength = 0;
    glGetProgramiv(shader->program, GL_ACTIVE_ATTRIBUTES, &attributeCount);
    glGetProgramiv(shader->program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

    char *name = new char[maxLength];

    for (int i = 0; i < attributeCount; ++i) {
        GLenum type;
        GLint length, size;
        glGetActiveAttrib(shader->program, i, 100, &length, &size, &type, name);

        GLint location = glGetAttribLocation(shader->program, name);

        Attribute *attr = new Attribute();

        attr->name = name;
        attr->location = location;
        attr->type = getAttributeType(type);
        attr->size = size;

        shader->addAttribute(attr);
    }

    delete[] name;
}

void ShaderUtil::fetchUniforms(Shader *shader)
{
    GLint uniformCount=0, maxLength=0;
    glGetProgramiv(shader->program, GL_ACTIVE_UNIFORMS, &uniformCount);
    glGetProgramiv(shader->program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

    int nUniforms = 0, nSamplers = 0;
    char *name = new char[maxLength];
    for (int i = 0; i < uniformCount; i++){
        GLenum type;
        GLint length, size;
        glGetActiveUniform(shader->program, i, maxLength, &length, &size, &type, name);

        {
            // Store all non-gl uniforms
            if (strncmp(name, "gl_", 3) != 0){
                char *bracket = strchr(name, '[');

                // not a array or just array[0]
                if (bracket == NULL
                    || (bracket[1] == '0' && bracket[2] == ']'))
                {
                    if (bracket){
                        *bracket = '\0';
                        length = (GLint) (bracket - name);
                    }

                    Uniform *uniform = new Uniform();

                    UniformType uniformType = getUniformType(type);
                    uniform->init(name, uniformType, glGetUniformLocation(shader->program, name), size);
                    if (uniformType >= UniformType::SAMPLER_1D && uniformType <= UniformType::SAMPLER_CUBE_ARRAY) {
                        RE_ASSERT(nSamplers + size < 15);

                        uniform->setData(TextureUnits + nSamplers);
                        nSamplers += size;
                    }

                    shader->addUniform(uniform);

                    nUniforms++;
                } else if (bracket != NULL && bracket[1] > '0') {
                    // not array[0], this will not happen normally

                    *bracket = '\0';

                    Uniform *uniform = shader->getUniform(name);

                    if (uniform != NULL) {
                        int count = atoi(bracket + 1) + 1;
                        if (count > uniform->getElementCount()){
                            uniform->setElementCount(count);
                        }
                    } else {
                        assert(false);
                    }
                }
            }
        }
    }

    delete[] name;
}

uint ShaderUtil::loadShader(const char *source, uint type)
{

#if GLES
    std::string precision;
    switch (type) {
    case GL_VERTEX_SHADER:
    {
        precision = "precision highp float;\n";
        break;
    }
    case GL_FRAGMENT_SHADER:
    {
        precision = "precision highp float;\n";
        break;
    }
    default:
        return -1;
    }

    std::string code = "#ifdef GL_ES\n" + precision + "#else\n#version 120\n#endif\n" + source;
#else
    std::string code = "#version 120\n";
    code += source;
#endif

    const char* sources = code.c_str();

    uint id = glCreateShader(type);
    glShaderSource(id, 1, (const GLchar **)&sources, 0);
    glCompileShader(id);

    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLsizei length;
        glGetShaderiv(id, GL_SHADER_SOURCE_LENGTH, &length);

        char* src = new char[length];

        glGetShaderSource(id, length, NULL, src);
        LogError("ShaderUtil: ERROR: Failed to compile shader: {}\n", src);

        delete[] src;

        GLint logLength = 0, charsWritten = 0;

        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength < 1)
            return 0;

        char *logBytes = (char*)malloc(logLength);
        glGetShaderInfoLog(id, logLength, &charsWritten, logBytes);

        if (type == GL_VERTEX_SHADER) {
            LogError("VS: {}", logBytes);

        } else {
            LogError("FS: {}", logBytes);
        }
        free(logBytes);

        abort();
    }

    return id;
}

void ShaderUtil::linkPrograme(Shader *shader)
{
    uint programID = glCreateProgram();
    shader->setProgram(programID);

    glAttachShader(programID, shader->getVertexShader());
    glAttachShader(programID, shader->getFragmentShader());

    glLinkProgram(programID);

    GLint linked = 0;
    glGetProgramiv(programID, GL_LINK_STATUS, &linked);

    if (!linked) {
        char *buf = new char[1024];
        int len = 0;

        glGetShaderInfoLog(shader->getVertexShader(), sizeof(buf), &len, buf);
        if (len > 0) {
            LogError("VS: {}", buf);
        }

        glGetShaderInfoLog(shader->getFragmentShader(), sizeof(buf), &len, buf);
        if (len > 0) {
            LogError("FS: {}", buf);
        }

        delete [] buf;
    } else {
        shader->setIsCompled(true);
    }
}

}
