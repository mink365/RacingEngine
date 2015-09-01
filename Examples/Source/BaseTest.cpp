#include "BaseTest.h"
#include "FeatureTestsApp.h"
#include "UI/Base/QuadStuffer.h"

#include "opengl.h"

int CheckShaderLinkError(GLint program) {
    GLint IsLinked;
    glGetProgramiv(program, GL_LINK_STATUS, (GLint *)&IsLinked);
    if(IsLinked==GL_FALSE)
    {
        LogError("Failed to link shader.");

        GLint maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        if(maxLength > 0)
        {
            char *pLinkInfoLog = new char[maxLength];
            glGetProgramInfoLog(program, maxLength, &maxLength, pLinkInfoLog);
            LogError("shader log: {}", pLinkInfoLog);

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

    ShaderUtil::instance().compileShader(shader.get());

    CheckShaderLinkError(shader->getProgram());

    if (name == "lights_lambert"
        || name == "lights_phong") {
        setLightShaderAttribute(shader);
    } else if (name == "depth_rgba") {
        setDepthRGBAAttribute(shader);
    } else {
        setPTCShaderAttribute(shader);
    }

    ShaderManager::instance().Register(shader);

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

void InitMeshInHardward(RenderElementPtr element, const string &shaderName) {
    auto mesh = element->getMesh();

    auto meshData = mesh->getMeshData();
    if (meshData == nullptr) {
        meshData = CreateDefaultMeshData();
        mesh->setMeshData(meshData);
    }

    UploadMeshToHardware(mesh);

    Shader::ptr shader = ShaderManager::instance().GetResource(shaderName);
    element->getMaterial()->setShder(shader);
}

void SetMeshData(EntityPtr node, GeometryPtr &geometry, Texture::ptr texture, const std::string& shaderName)
{
    RenderElementPtr element = node->getComponent<RenderElement>();
    MeshPtr mesh = element->getMesh();

    mesh->setGeometry(geometry);

    InitMeshInHardward(element, shaderName);

    RE_ASSERT(texture != nullptr);
    SamplerParameter::ptr unit = element->getMaterial()->getSampler("textureSampler");
    if (unit != nullptr) {
        unit->setTexture(texture);
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
        if (queue == RenderQueues::UI) {
            return false;
        }
        return true;
    });
}

void BaseTest::Init()
{

}

void BaseTest::Update()
{

}
