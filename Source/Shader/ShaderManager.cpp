#include "ShaderManager.h"

namespace re {

ShaderManager::ShaderManager()
{
}

void ShaderManager::registerShader(Shader::type shader)
{
    this->shaders[shader->getName()] = shader;
}

Shader::type ShaderManager::getShader(const string &key)
{
    return this->shaders[key];
}

void ShaderManager::loadShaders()
{

}

void ShaderManager::reloadShaders()
{

}

}
