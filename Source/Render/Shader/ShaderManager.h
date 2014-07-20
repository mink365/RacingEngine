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
    void registerShader(Shader::ptr shader);
    Shader::ptr getShader(const std::string& key);

    void loadShaders();
    void reloadShaders();

private:
    std::map<const std::string, Shader::ptr> shaders;
};

} // namespace re

#endif // SHADERMANAGER_H
