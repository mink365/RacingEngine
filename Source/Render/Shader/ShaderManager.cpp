#include "ShaderManager.h"

namespace re {

ShaderManager::ShaderManager()
{
}

void ShaderManager::registerShader(Shader::ptr shader)
{
    this->shaders[shader->getName()] = shader;
}

Shader::ptr ShaderManager::getShader(const string &key)
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
