#include "RenderToTextureTest.h"

#include "opengl.h"

#include "ShapeGenerater.h"
#include "Render/Renderer/Renderer.h"
#include "Render/RenderTarget.h"

#include "Math/Vector.h"

RenderToTextureTest::RenderToTextureTest()
{
    this->name = "RenderToTextureTest";
}

GLint m_i32OriginalFbo;
int m_i32TexSize;
GLuint m_uDepthBuffer, m_uiTextureToRenderTo, m_uFBO;
bool CreateFBO();

SceneNodePtr node;
MeshPtr mesh;
Renderer* renderer;
RenderManager* renderManager;
RenderTargetPtr renderTarget;

GLint texID = 0;

void RenderToTextureTest::Init()
{
    CreateFBO();

    renderManager = &(SceneManager::getInstance().getRenderManager());
    renderer = &(renderManager->getRenderer());

    TextureParser::getInstance().addTextures("Textures/NormalMap", "png|jpg");

    auto texture = TextureManager::getInstance().getTexture("diffuse");
    texID = texture->getGlID();

    GeometryPtr geometry = nullptr;
    geometry = ShapeGenerater::getInstance().CreateCylinder(50, 50, 100, 10, 10);
    mesh = ShapeGenerater::getInstance().CreateMesh(geometry, texture);
    InitMeshInHardward(mesh);
    mesh->getMaterial()->setQueueID(51);

    node = std::make_shared<SceneNode>();
    AddMeshToNode(node, mesh);

    rootNode->addChild(node);

    // create a render target
    renderTarget = std::make_shared<RenderTarget>();
    renderTarget->setHasDepthBuffer(true);
    renderTarget->setHasStencilBuffer(false);

    auto size = renderTarget->getSize();

    renderer->setupRenderTarget(*renderTarget);
}

void RenderToTextureTest::Update(float dt)
{
    RenderScene1();
}

void RenderToTextureTest::RenderScene1() {
    GLint fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
    LOG_E("FBO: %d", fbo);

    renderer->bindRenderTarget(*renderTarget);
//    glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);
    renderer->setViewport(Rect(0, 0, 512, 512));

    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
    LOG_E("FBO2: %d", fbo);

    renderer->setViewMatrix(camera->getViewMatrix());
    renderer->setProjectionMatrix(camera->getProjectionMatrix());

//    renderer->renderMesh(*(mesh->getGeometry()));
    mesh->getMaterial()->getPass(0)->getTextureUnit(0)->getActivityTexture()->setGlID(texID);
    renderManager->renderMesh(mesh);

    renderer->resetRenderTarget();
    glClearColor(1.0f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer->setViewport(Rect(0, 0, 512, 512));
    renderer->setViewMatrix(camera->getViewMatrix());
    renderer->setProjectionMatrix(camera->getProjectionMatrix());

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
    LOG_E("FBO3: %d", fbo);

    mesh->getMaterial()->getPass(0)->getTextureUnit(0)->getActivityTexture()->setGlID(renderTarget->getTexture()->getGlID());
//     mesh->getMaterial()->getPass(0)->getTextureUnit(0)->getActivityTexture()->setGlID(m_uiTextureToRenderTo);
    renderManager->renderMesh(mesh);
}

void RenderScene() {
//    // render to tex
//    {
//        // Bind our FBO
////        if (m_bUseMultisampled)
////            glBindFramebuffer(GL_FRAMEBUFFER, m_uFBOMultisampled);
////        else
//            glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);

//        // Setup the Viewport to the dimensions of the texture
//        glViewport(0, 0, m_i32TexSize, m_i32TexSize);

//        // Set the colour to clear our texture to
//        glClearColor(0.8f, 1.0f, 0.6f, 1.0f);

//        // Clear the colour and depth buffer of our FBO / PBuffer surface
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



//        // Render our objects as we usually would
//        mWorld = PVRTMat4::RotationY(m_fAngleY);

//        // Pass the light direction in model space to the shader
//        vLightDir = mWorld.inverse() * vLightDirection;

//        vLightDirModel = PVRTVec3(&vLightDir.x);
//        vLightDirModel.normalize();

//        glUniform3fv(m_ShaderProgram.auiLoc[eLightDirection], 1, &vLightDirModel.x);

//        // Set the model-view-projection matrix
//        mMVP = m_mR2TProjection * m_mR2TView * mWorld;
//        glUniformMatrix4fv(m_ShaderProgram.auiLoc[eMVPMatrix], 1, GL_FALSE, mMVP.f);

//        // Bind the mask's texture
//        glBindTexture(GL_TEXTURE_2D, m_uiTextureID);

//        // Draw our mask
//        DrawMesh(m_Scene.pNode[0].nIdx);


//        // We are done with rendering to our FBO so switch back to the back buffer.
//        glBindFramebuffer(GL_FRAMEBUFFER, m_i32OriginalFbo);
//    }

//    // render the tex

//    {
//        // Set the clear colour
//        glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

//        // Clear the colour and depth buffer
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        // Setup the Viewport to the dimensions of the screen
//        glViewport(0, 0, PVRShellGet(prefWidth), PVRShellGet(prefHeight));

//        // Get the node model matrix
//        mWorld = m_Scene.GetWorldMatrix(m_Scene.pNode[1]);

//        // Pass the light direction in model space to the shader
//        vLightDir = mWorld.inverse() * vLightDirection;

//        vLightDirModel = PVRTVec3(&vLightDir.x);
//        vLightDirModel.normalize();

//        glUniform3fv(m_ShaderProgram.auiLoc[eLightDirection], 1, &vLightDirModel.x);

//        // Set the model-view-projection matrix
//        mMVP = m_mProjection * m_mView * mWorld ;
//        glUniformMatrix4fv(m_ShaderProgram.auiLoc[eMVPMatrix], 1, GL_FALSE, mMVP.f);

//        // Bind our texture that we have rendered to
//        if (m_bUseMultisampled)
//            glBindTexture(GL_TEXTURE_2D, m_uiTextureToRenderToMultisampled);
//        else
//            glBindTexture(GL_TEXTURE_2D, m_uiTextureToRenderTo);

//        // Draw our textured cube
//        DrawMesh(m_Scene.pNode[1].nIdx);

//        // Safely disable the vertex attribute arrays
//        glDisableVertexAttribArray(eVERTEX_ARRAY);
//        glDisableVertexAttribArray(eNORMAL_ARRAY);
//        glDisableVertexAttribArray(eTEXCOORD_ARRAY);

//        // Unbind our VBOs
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//    }
}

bool CreateFBO()
{
//    // Find the largest square power of two texture that fits into the viewport
//    m_i32TexSize = 1;
//    int iSize = PVRT_MIN(PVRShellGet(prefWidth), PVRShellGet(prefHeight));
//    while (m_i32TexSize * 2 < iSize) m_i32TexSize *= 2;
    m_i32TexSize = 512;

    // Get the currently bound frame buffer object. On most platforms this just gives 0.
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_i32OriginalFbo);

    // Generate and bind a render buffer which will become a depth buffer shared between our two FBOs
    glGenRenderbuffers(1, &m_uDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_uDepthBuffer);

    /*
        Currently it is unknown to GL that we want our new render buffer to be a depth buffer.
        glRenderbufferStorage will fix this and in this case will allocate a depth buffer
        m_i32TexSize by m_i32TexSize.
    */

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_i32TexSize, m_i32TexSize);

    // Create a texture for rendering to
    glGenTextures(1, &m_uiTextureToRenderTo);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureToRenderTo);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_i32TexSize, m_i32TexSize, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 0);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create the object that will allow us to render to the aforementioned texture
    glGenFramebuffers(1, &m_uFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uFBO);

    // Attach the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiTextureToRenderTo, 0);

    // Attach the depth buffer we created earlier to our FBO.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uDepthBuffer);

    // Check that our FBO creation was successful
    GLuint uStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if(uStatus != GL_FRAMEBUFFER_COMPLETE)
    {
//        PVRShellSet(prefExitMessage, "ERROR: Failed to initialise FBO");
        assert(false);
        return false;
    }

    // Clear the colour and depth buffers for the FBO / PBuffer surface
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Unbind the frame buffer object so rendering returns back to the backbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_i32OriginalFbo);

    return true;
}
