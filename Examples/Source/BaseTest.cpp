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
    vertAttr->setType(AttributeFormat::FLOAT);
    vertAttr->setSize(3);
    vertAttr->setStride(sizeof(FbxVertex));
    vertAttr->setOffset(0);

    Attribute *uvAttr = shader->getAttribute("aTexCoord");
    uvAttr->setType(AttributeFormat::FLOAT);
    uvAttr->setSize(2);
    uvAttr->setStride(sizeof(FbxVertex));
    uvAttr->setOffset((3) * 4);

    Attribute *normalAttr = shader->getAttribute("aNormal");
    normalAttr->setType(AttributeFormat::FLOAT);
    normalAttr->setSize(3);
    normalAttr->setStride(sizeof(FbxVertex));
    normalAttr->setOffset((5) * 4);

    Attribute *colorAttr = shader->getAttribute("aColor");
    colorAttr->setType(AttributeFormat::FLOAT);
    colorAttr->setSize(4);
    colorAttr->setStride(sizeof(FbxVertex));
    colorAttr->setOffset((8) * 4);
}

void setPTCShaderAttribute(Shader::ptr& shader) {
    if (shader->getAttribute("aPosition")) {
        Attribute *vertAttr = shader->getAttribute("aPosition");
        vertAttr->setType(AttributeFormat::FLOAT);
        vertAttr->setSize(3);
        vertAttr->setStride(sizeof(FbxVertex));
        vertAttr->setOffset(0);

        Attribute *uvAttr = shader->getAttribute("aTexCoord");
        if (uvAttr) {
            uvAttr->setType(AttributeFormat::FLOAT);
            uvAttr->setSize(2);
            uvAttr->setStride(sizeof(FbxVertex));
            uvAttr->setOffset((3) * 4);
        }

        Attribute *normalAttr = shader->getAttribute("aNormal");
        if (normalAttr) {
            normalAttr->setType(AttributeFormat::FLOAT);
            normalAttr->setSize(3);
            normalAttr->setStride(sizeof(FbxVertex));
            normalAttr->setOffset((5) * 4);
        }

        Attribute *colorAttr = shader->getAttribute("aColor");
        if (colorAttr) {
            colorAttr->setType(AttributeFormat::FLOAT);
            colorAttr->setSize(4);
            colorAttr->setStride(sizeof(FbxVertex));
            colorAttr->setOffset((8) * 4);
        }
    } else {
        Attribute *vertAttr = shader->getAttribute("inVertex");
        vertAttr->setType(AttributeFormat::FLOAT);
        vertAttr->setSize(3);
        vertAttr->setStride(sizeof(FbxVertex));
        vertAttr->setOffset(0);

        Attribute *uvAttr = shader->getAttribute("inTexCoord");
        if (uvAttr) {
            uvAttr->setType(AttributeFormat::FLOAT);
            uvAttr->setSize(2);
            uvAttr->setStride(sizeof(FbxVertex));
            uvAttr->setOffset((3) * 4);
        }

        Attribute *normalAttr = shader->getAttribute("inNormal");
        if (normalAttr) {
            normalAttr->setType(AttributeFormat::FLOAT);
            normalAttr->setSize(3);
            normalAttr->setStride(sizeof(FbxVertex));
            normalAttr->setOffset((5) * 4);
        }

        Attribute *colorAttr = shader->getAttribute("inColor");
        if (colorAttr) {
            colorAttr->setType(AttributeFormat::FLOAT);
            colorAttr->setSize(4);
            colorAttr->setStride(sizeof(FbxVertex));
            colorAttr->setOffset((8) * 4);
        }
    }
}

void setDepthRGBAAttribute(Shader::ptr& shader) {
    Attribute *vertAttr = shader->getAttribute("aPosition");
    vertAttr->setType(AttributeFormat::FLOAT);
    vertAttr->setSize(3);
    vertAttr->setStride(sizeof(FbxVertex));
    vertAttr->setOffset(0);
}

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
    } else if (name == "depth_rgba") {
        setDepthRGBAAttribute(shader);
    } else {
        setPTCShaderAttribute(shader);
    }

    ShaderManager::getInstance().registerShader(shader);

    return 1;
}

std::string loadFile(const char *fname)
{
    FilePtr file = FileSystem::getInstance().getFile(fname);
    ByteBufferPtr buf = file->read();

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
    BufferObjectUtil::getInstance().loadGeometryToHardware(*(mesh.get()));

    Shader::ptr shader = ShaderManager::getInstance().getShader(shaderName);
    mesh->getMaterial()->setShder(shader);
}

void SetMeshData(NodePtr node, GeometryPtr &geometry, Texture::ptr texture, const std::string& shaderName)
{
    MeshPtr mesh = node->getComponent<Mesh>();
    mesh->init();

    mesh->setGeometry(geometry);

    TextureUnitState::ptr unit = node->getComponent<re::Material>()->getPass(0)->getTextureUnit(0);
    unit->setUVstate(0, 0, 1, 1, 0);

    if (texture == nullptr) {
        texture = TextureManager::getInstance().getTexture("girl");
    }
    unit->setTexture(texture);

    InitMeshInHardward(mesh, shaderName);
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

    this->setDefaultEnv();

    this->Init();
}

string BaseTest::getName() const
{
    return this->name;
}

void BaseTest::setDefaultEnv()
{
    this->camera->setDepthField(10, 1320);
    this->camera->setView(Vec3(100, 0, 300), Vec3(0, 0, 0), Vec3(0, 1, 0));
    this->camera->setQueueCullFunc([](int queue) {
        if (queue == RENDER_QUEUE_UI) {
            return false;
        }
        return true;
    });


}

void BaseTest::Init()
{

}

void BaseTest::Update(float dt)
{

}
