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

#include "opengl.h"

using namespace std;
using namespace re;

class FBXTestApp : public Application {
public:
    FBXTestApp();
};

void initResource();
void updateMatrix(bool isAnim);
void TestFont();
void TestUI();

#endif // FBX_RENDER_TEST_H
