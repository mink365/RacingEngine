#ifndef BASETEST_H
#define BASETEST_H

#include "PreDeclare.h"

#include "Scene/SceneManager.h"
#include "Scene/Node.h"
#include "Render/Mesh.h"
#include "Scene/RenderElement.h"
#include "Render/Material/Material.h"
#include "Camera/Camera.h"
#include "UI/Scene.h"
#include "UI/Widget/Button.h"
#include "UI/Layout/LayoutUtil.h"
#include "FileSystem/FileSystem.h"
#include "Texture/TextureManager.h"
#include "Texture/TextureParser.h"
#include "Shader/ShaderUtil.h"
#include "Font/TextureAtlas.h"
#include "Font/FreeTypeUtil.h"

#include "Platform/GameHub.h"
#include "FileSystem/File.h"
#include "FileSystem/FileSystem.h"
#include "Render/BufferObject/BufferObjectUtil.h"
#include "UI/Manager/UIManager.h"
#include "Util/ComponentFactory.h"

using namespace re;
using namespace re::ui;

class FeatureTestsApp;
using UIManagerPtr = ComponentHandle<ui::UIManager>;

int LoadShader(const std::string& name, const std::string& pfilePath_vs, const std::string& pfilePath_fs);

inline ComponentHandle<Button> CreateImageButton(const string& texDefault, const string& texPress, const string& texDis)
{
    // TODO: init ImageButtonData

    auto entity = CreateEntity();
    auto button = entity->addComponent<Button>();
    ImageButtonData data{texDefault, texPress, texDis};
    button->getData<ButtonType::SpriteSwap>() = data;
    entity->switchState(EntityState::Awaked);

    return button;
}

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
    UIManagerPtr stage;
};

void InitMeshInHardward(RenderElementPtr element, const std::string& shaderName="Shader_PTC");
void SetMeshData(EntityPtr node, GeometryPtr &geometry, Texture::ptr texture=nullptr, const std::string& shaderName="Shader_PTC");

#endif // BASETEST_H
