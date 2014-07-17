#ifndef BASETEST_H
#define BASETEST_H

#include "PredefineType.h"

#include "Scene/SceneManager.h"
#include "Scene/SceneNode.h"
#include "Camera/Camera.h"
#include "UI/Scene.h"
#include "FileSystem/FileSystem.h"
#include "Texture/TextureManager.h"
#include "Texture/TextureParser.h"
#include "Shader/ShaderUtil.h"
#include "Shader/ShaderManager.h"
#include "Font/FontManager.h"
#include "Font/TextureAtlas.h"
#include "Font/FreeTypeUtil.h"

#include "FileSystem/File.h"
#include "FileSystem/FileSystem.h"
#include "Render/BufferObject/BufferObjectUtil.h"
#include "Shader/ShaderManager.h"
#include "UI/Manager/UISceneManager.h"

using namespace re;

class FeatureTestsApp;

class BaseTest
{
public:
    BaseTest();
    virtual ~BaseTest();

    void init(FeatureTestsApp& app);

    virtual void Init();
    virtual void Update(float dt);

protected:
    SceneNodePtr rootNode;
    CameraPtr camera;
    std::shared_ptr<ui::UISceneManager> stage;
};

void InitMeshInHardward(MeshPtr mesh);
void AddMeshToNode(SceneNodePtr node, MeshPtr mesh);

#endif // BASETEST_H
