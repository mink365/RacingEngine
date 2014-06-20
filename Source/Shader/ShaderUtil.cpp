#include "ShaderUtil.h"

#include "platform.h"

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
void *uniformFuncs[UNIFORM_TYPE_COUNT];

AttributeFormat getAttributeType(GLenum type) {
    switch(type) {
        case GL_FLOAT: return ATTR_FORMAT_FLOAT;
        case GL_UNSIGNED_BYTE: return ATTR_FORMAT_UBYTE;
    }

    return (AttributeFormat) - 1;
}

UniformType getUniformType(GLenum type) {
    switch (type){
        case GL_FLOAT:          return UNIFORM_FLOAT;
        case GL_FLOAT_VEC2: return UNIFORM_VEC2;
        case GL_FLOAT_VEC3: return UNIFORM_VEC3;
        case GL_FLOAT_VEC4: return UNIFORM_VEC4;
        case GL_INT:            return UNIFORM_INT;
        case GL_INT_VEC2:   return UNIFORM_IVEC2;
        case GL_INT_VEC3:   return UNIFORM_IVEC3;
        case GL_INT_VEC4:   return UNIFORM_IVEC4;
        case GL_BOOL:       return UNIFORM_BOOL;
        case GL_BOOL_VEC2:  return UNIFORM_BVEC2;
        case GL_BOOL_VEC3:  return UNIFORM_BVEC3;
        case GL_BOOL_VEC4:  return UNIFORM_BVEC4;
        case GL_FLOAT_MAT2: return UNIFORM_MAT2;
        case GL_FLOAT_MAT3: return UNIFORM_MAT3;
        case GL_FLOAT_MAT4: return UNIFORM_MAT4;
    }

    return (UniformType) -1;
}

ShaderUtil::ShaderUtil()
{
    uniformFuncs[UNIFORM_FLOAT] = (void *) glUniform1fv;
    uniformFuncs[UNIFORM_VEC2]  = (void *) glUniform2fv;
    uniformFuncs[UNIFORM_VEC3]  = (void *) glUniform3fv;
    uniformFuncs[UNIFORM_VEC4]  = (void *) glUniform4fv;
    uniformFuncs[UNIFORM_INT]   = (void *) glUniform1iv;
    uniformFuncs[UNIFORM_IVEC2] = (void *) glUniform2iv;
    uniformFuncs[UNIFORM_IVEC3] = (void *) glUniform3iv;
    uniformFuncs[UNIFORM_IVEC4] = (void *) glUniform4iv;
    uniformFuncs[UNIFORM_BOOL]  = (void *) glUniform1iv;
    uniformFuncs[UNIFORM_BVEC2] = (void *) glUniform2iv;
    uniformFuncs[UNIFORM_BVEC3] = (void *) glUniform3iv;
    uniformFuncs[UNIFORM_BVEC4] = (void *) glUniform4iv;
    uniformFuncs[UNIFORM_MAT2]  = (void *) glUniformMatrix2fv;
    uniformFuncs[UNIFORM_MAT3]  = (void *) glUniformMatrix3fv;
    uniformFuncs[UNIFORM_MAT4]  = (void *) glUniformMatrix4fv;
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

//const char* vertexShaderLog(int id)
//{
//    return logForOpenGLObject(id, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
//}

//const char* fragmentShaderLog(int id)
//{
//    return logForOpenGLObject(id, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
//}

//const char* programLog(int id)
//{
//    return logForOpenGLObject(id, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
//}

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

    std::vector<Attribute *> attributes = shader->getAttributes();
    std::vector<Attribute *>::iterator iter;
    for (iter = attributes.begin(); iter != attributes.end(); ++iter) {
        Attribute* attribute = *iter;

        this->applyAttributeToHardware(attribute);
    }

    std::vector<Uniform *> uniforms = shader->getUniforms();
    std::vector<Uniform *>::iterator iter_uniform;
    for (iter_uniform = uniforms.begin(); iter_uniform != uniforms.end(); ++iter_uniform) {
        Uniform* uniform = *iter_uniform;

        this->applyUniformToHardware(uniform);
    }
}

void ShaderUtil::applyAttributeToHardware(Attribute *attr)
{
    glEnableVertexAttribArray(attr->location);
    glVertexAttribPointer(attr->location, attr->size, glTypes[attr->type], attr->normalized, attr->stride, (void *)attr->offset);
}

void ShaderUtil::applyUniformToHardware(Uniform *uniform)
{
    if (uniform->type >= UNIFORM_MAT2){
        int cal = 2 + uniform->type - UNIFORM_MAT2;
        int size = cal * cal;
        float mat[size];
        for (int i = 0; i < cal; ++i) {
            for (int j = 0; j < cal; ++j) {
                mat[i * cal + j] = uniform->data[j * cal + i];
            }
        }

        ((UNIFORM_MAT_FUNC) uniformFuncs[uniform->type])(uniform->location, uniform->nElements, GL_FALSE, mat);
    } else {
        ((UNIFORM_FUNC) uniformFuncs[uniform->type])(uniform->location, uniform->nElements, (float *) uniform->data);
    }
}

void ShaderUtil::fetchAttributes(Shader *shader)
{
    GLint attributeCount, maxLength;
    glGetProgramiv(shader->program, GL_ACTIVE_ATTRIBUTES, &attributeCount);
    glGetProgramiv(shader->program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

    char *name = new char[maxLength];

    for (int i = 0; i < attributeCount; ++i) {
        GLenum type;
        GLint length, size;
        glGetActiveAttrib(shader->program, i, 100, &length, &size, &type, name);

        GLint location = glGetAttribLocation(shader->program, name);

        Attribute *attr = new Attribute();

        // TODO: char *對string的赋值到底是复制还是直接指针指向？
        // 从运行结果上说是复制啊！！
//        char *attr_name = new char[length + 1];
//        strncpy(attr_name, name, length);

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
    GLint uniformCount, maxLength;
    glGetProgramiv(shader->program, GL_ACTIVE_UNIFORMS, &uniformCount);
    glGetProgramiv(shader->program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

    int nSamplers = 0;
    int nUniforms = 0;
    char *name = new char[maxLength];
    for (int i = 0; i < uniformCount; i++){
        GLenum type;
        GLint length, size;
        glGetActiveUniform(shader->program, i, maxLength, &length, &size, &type, name);

        if (type >= GL_SAMPLER_1D && type <= GL_SAMPLER_2D_RECT_SHADOW){
            // Assign samplers to image units
            GLint location = glGetUniformLocation(shader->program, name);
            glUniform1i(location, nSamplers);

            Sampler *sampler = new Sampler(name, nSamplers);
            shader->samplers.push_back(sampler);

            nSamplers++;
        } else {
            // Store all non-gl uniforms
            if (strncmp(name, "gl_", 3) != 0){
                char *bracket = strchr(name, '[');

                // 非数组/array或者是数组的第一个元素
                if (bracket == NULL
                        || (bracket[1] == '0' && bracket[2] == ']')){
                    if (bracket){
                        *bracket = '\0';
                        length = (GLint) (bracket - name);
                    }

                    Uniform *uniform = new Uniform();
                    // TODO: 内存在堆还是栈上分配？是复制还是直接使用了原始的内存区？
//                    uniform->name += name;
//                    char *uniform_name = new char[length + 1];
//                    strcpy(uniform_name, name);
                    // 根据http://en.cppreference.com/w/cpp/string/basic_string/operator%3D的
                    // 说法，直接使用char*替换了string的内存区，那么，内存会泄漏吗？
                    uniform->name = name;

                    uniform->location = glGetUniformLocation(shader->program, name);
                    uniform->type = getUniformType(type);
                    uniform->nElements = size;

                    shader->addUniform(uniform);

                    nUniforms++;
                } else if (bracket != NULL && bracket[1] > '0'){
                    // 处理uniform数组的其他元素

                    *bracket = '\0'; // 直接给出一个字符串结束符号，以截断字符串，只保留名字

                    Uniform *uniform = shader->getUniform(name);

                    if (uniform != NULL) {
                        int count = atoi(bracket + 1) + 1;
                        if (count > uniform->nElements){
                            uniform->nElements = count;
                        }
                    } else {
                        // 这其实是个错误
                    }
                }
            }
        }
    }

    delete[] name;
}

uint ShaderUtil::loadShader(const char *source, uint type)
{
    uint id;

    switch (type) {
    case GL_VERTEX_SHADER:
        // create vertex shader
        id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(id, 1, (const GLchar **)&source, 0);
        glCompileShader(id);
        break;
    case GL_FRAGMENT_SHADER:
        // create fragment shader
        id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(id, 1, (const GLchar **)&source, 0);
        glCompileShader(id);
        break;
    default:
        return -1;
    }

    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLsizei length;
        glGetShaderiv(id, GL_SHADER_SOURCE_LENGTH, &length);

        char* src = new char[length];

        glGetShaderSource(id, length, NULL, src);
        LOG_E("ShaderUtil: ERROR: Failed to compile shader:\n%s", src);

        delete[] src;


        GLint logLength = 0, charsWritten = 0;

        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength < 1)
            return 0;

        char *logBytes = (char*)malloc(logLength);
        glGetShaderInfoLog(id, logLength, &charsWritten, logBytes);

        if (type == GL_VERTEX_SHADER) {
            LOG_E("VS: %s", logBytes);

        } else {
            LOG_E("FS: %s", logBytes);
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

    GLint linked;
    glGetProgramiv(programID, GL_LINK_STATUS, &linked);

    if (!linked) {
        char *buf = new char[1024];
        int len = 0;

        glGetShaderInfoLog(shader->getVertexShader(), sizeof(buf), &len, buf);
        if (len > 0) {
            LOG_E("VS: %s\n", buf);
        }

        glGetShaderInfoLog(shader->getFragmentShader(), sizeof(buf), &len, buf);
        if (len > 0) {
            LOG_E("FS: %s\n", buf);
        }

        delete [] buf;
    } else {
        shader->setIsCompled(true);
    }
}

}
