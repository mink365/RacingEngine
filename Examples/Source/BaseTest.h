#ifndef BASETEST_H
#define BASETEST_H

#include "PreDeclare.h"

#include "Scene/SceneManager.h"
#include "Scene/Node.h"
#include "Scene/Mesh.h"
#include "Render/Material/Material.h"
#include "Camera/Camera.h"
#include "UI/Scene.h"
#include "UI/Layout/LayoutUtil.h"
#include "FileSystem/FileSystem.h"
#include "Texture/TextureManager.h"
#include "Texture/TextureParser.h"
#include "Shader/ShaderUtil.h"
#include "Shader/ShaderManager.h"
#include "Font/FontManager.h"
#include "Font/TextureAtlas.h"
#include "Font/FreeTypeUtil.h"

#include "Platform/GameHub.h"
#include "FileSystem/File.h"
#include "FileSystem/FileSystem.h"
#include "Render/BufferObject/BufferObjectUtil.h"
#include "Shader/ShaderManager.h"
#include "UI/Manager/UIManager.h"
#include "Util/ComponentFactory.h"

using namespace re;

class FeatureTestsApp;

int LoadShader(const std::string& name, const std::string& pfilePath_vs, const std::string& pfilePath_fs);

class BaseTest
{
public:
    BaseTest();
    virtual ~BaseTest();

    void init(FeatureTestsApp& app);

    std::string getName() const;

    void setDefaultEnv();

    virtual void Init();
    virtual void Update();

protected:
    std::string name;

    NodePtr rootNode;
    CameraPtr camera;
    std::shared_ptr<ui::UIManager> stage;
};

void InitMeshInHardward(MeshPtr mesh, const std::string& shaderName="Shader_PTC");
void SetMeshData(NodePtr node, GeometryPtr &geometry, Texture::ptr texture=nullptr, const std::string& shaderName="Shader_PTC");

#endif // BASETEST_H
