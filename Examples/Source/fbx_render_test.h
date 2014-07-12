#ifndef FBX_RENDER_TEST_H
#define FBX_RENDER_TEST_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader/ShaderUtil.h"
#include "Shader/ShaderManager.h"
#include "Camera/Camera.h"
#include "Render/BufferObject/BufferObjectUtil.h"
#include "Scene/Mesh.h"
#include "Tools/ModelLoader/fbx/FbxParser.h"
#include "Texture/TextureManager.h"
#include "Scene/SceneManager.h"
#include "Animation/SkeletonController.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/BoneNode.h"
#include "Texture/TextureParser.h"
#include "FileSystem/FileSystem.h"
#include "Platform/linux/Application.h"
#include "UI/Manager/ViewFactory.h"

#include "opengl.h"

using namespace std;
using namespace re;

class FBXTestApp : public Application {
public:
    FBXTestApp();

    virtual bool initEnvironment();
    virtual void onEnterForeground();
    virtual void onExitForeground();
};

void initResource();
void updateMatrix(bool isAnim);
void TestFont();
void TestUI();

class WindowFactory : public IWindowFactory
{
public:
    virtual std::shared_ptr<Window> createView(const string& name) override;
};

class SceneFactory : public ISceneFactory
{
public:
    virtual std::shared_ptr<Scene> createView(const string& name) override;
};

#endif // FBX_RENDER_TEST_H
