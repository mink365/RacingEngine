#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "platform.h"
#include <map>

#include "Base/Singleton.h"
#include "Shader/Shader.h"

namespace re {

class ShaderManager : public Singleton<ShaderManager>
{
    friend class Singleton;
private:
    ShaderManager();

public:
    void registerShader(Shader::type shader);
    Shader::type getShader(const std::string& key);

    void loadShaders();
    void reloadShaders();

private:
    std::map<const std::string, Shader::type> shaders;
};

} // namespace re

#endif // SHADERMANAGER_H
