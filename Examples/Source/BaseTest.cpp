#include "BaseTest.h"
#include "FeatureTestsApp.h"

#include "opengl.h"

int CheckShaderLinkError(GLint program) {
    GLint IsLinked;
    glGetProgramiv(program, GL_LINK_STATUS, (GLint *)&IsLinked);
    if(IsLinked==GL_FALSE)
    {
        LOG_E("Failed to link shader.");

        GLint maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        if(maxLength > 0)
        {
            char *pLinkInfoLog = new char[maxLength];
            glGetProgramInfoLog(program, maxLength, &maxLength, pLinkInfoLog);
            LOG_E("shader log: %s\n", pLinkInfoLog);

            delete [] pLinkInfoLog;
        }

        return -1;
    }

    return 0;
}

void setLightShaderAttribute(Shader::ptr& shader) {
    // set vertex attribute
    Attribute *vertAttr = shader->getAttribute("aPosition");
    vertAttr->setType(ATTR_FORMAT_FLOAT);
    vertAttr->setSize(3);
    vertAttr->setStride(sizeof(Vertex));
    vertAttr->setOffset(0);

    Attribute *uvAttr = shader->getAttribute("aTexCoord");
    uvAttr->setType(ATTR_FORMAT_FLOAT);
    uvAttr->setSize(2);
    uvAttr->setStride(sizeof(Vertex));
    uvAttr->setOffset((3) * 4);

    Attribute *colorAttr = shader->getAttribute("aColor");
    colorAttr->setType(ATTR_FORMAT_FLOAT);
    colorAttr->setSize(4);
    colorAttr->setStride(sizeof(Vertex));
    colorAttr->setOffset((8) * 4);

    Attribute *normalAttr = shader->getAttribute("aNormal");
    normalAttr->setType(ATTR_FORMAT_FLOAT);
    normalAttr->setSize(3);
    normalAttr->setStride(sizeof(Vertex));
    normalAttr->setOffset((5) * 4);
}

void setPTCShaderAttribute(Shader::ptr& shader) {
    // set vertex attribute
    Attribute *vertAttr = shader->getAttribute("aPosition");
    vertAttr->setType(ATTR_FORMAT_FLOAT);
    vertAttr->setSize(3);
    vertAttr->setStride(sizeof(Vertex));
    vertAttr->setOffset(0);

    Attribute *uvAttr = shader->getAttribute("aTexCoord");
    uvAttr->setType(ATTR_FORMAT_FLOAT);
    uvAttr->setSize(2);
    uvAttr->setStride(sizeof(Vertex));
    uvAttr->setOffset((3) * 4);

    Attribute *colorAttr = shader->getAttribute("aColor");
    colorAttr->setType(ATTR_FORMAT_FLOAT);
    colorAttr->setSize(4);
    colorAttr->setStride(sizeof(Vertex));
    colorAttr->setOffset((8) * 4);
}

Camera camera;
int LoadShaderData(const std::string& name, const std::string& vs, const std::string& fs) {
    Shader::ptr shader = Shader::create();
    shader->setName(name);

    shader->setVertexSource(vs);
    shader->setFragmentSource(fs);

    ShaderUtil::getInstance().compileShader(shader.get());

    CheckShaderLinkError(shader->getProgram());

    if (name == "lights_lambert"
        || name == "lights_phong") {
        setLightShaderAttribute(shader);
    } else {
        setPTCShaderAttribute(shader);
    }

//    shader.getUniform("model")->setData((float*)model);
//    shader->getUniform("view")->setData((float*)camera.getViewMatrix());
//    shader->getUniform("projection")->setData((float*)camera.getProjectionMatrix());

    ShaderManager::getInstance().registerShader(shader);

    return 1;
}

std::string loadFile(const char *fname)
{
    FilePtr file = FileSystem::getInstance().openFile(fname);
    Buffer::ptr buf = file->read();

    string data((char*)(buf->getData()), buf->getSize());
    return data;
}

int LoadShader(const std::string& name, const std::string& pfilePath_vs, const std::string& pfilePath_fs)
{
    // load shaders & get length of each
    std::string vertexShaderString = loadFile(pfilePath_vs.c_str());
    std::string fragmentShaderString = loadFile(pfilePath_fs.c_str());

    return LoadShaderData(name, vertexShaderString, fragmentShaderString);
}

void InitMeshInHardward(MeshPtr mesh, const string &shaderName) {
    Geometry::ptr geometry = mesh->getGeometry();
    BufferObjectUtil::getInstance().loadGeometryToHardware(*(geometry.get()));

    Shader::ptr shader = ShaderManager::getInstance().getShader(shaderName);
    mesh->getMaterial()->setShder(shader);
}

void AddMeshToNode(SceneNodePtr node, MeshPtr mesh) {
    node->setNodeAttribute(mesh->clone());

    for (auto child : node->getChildren()) {
        AddMeshToNode(dynamic_pointer_cast<SceneNode>(child), mesh);
    }
}

BaseTest::BaseTest()
{
}

BaseTest::~BaseTest()
{

}

void BaseTest::init(FeatureTestsApp &app)
{
    this->camera = app.presCamera;
    this->rootNode = app.rootNode;
    this->stage = app.stage;

    this->Init();
}

void BaseTest::Init()
{

}

void BaseTest::Update(float dt)
{

}
