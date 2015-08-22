#include "Bumpmap.h"

#include "ShapeGenerater.h"
#include "RenderElement.h"

Bumpmap::Bumpmap()
{
    this->name = "NormalMapTest";
}

struct NormalMapVertex {
    Vec3 xyz;
    Uv uv;
    Vec3 normal;
    Vec4 tangent;
};

void Bumpmap::Init()
{
    std::string shaderDir = "Shaders/";

    LoadShader("normal_map", shaderDir + "bumpmap.vsh",
                             shaderDir + "bumpmap.fsh");

    TextureParser::getInstance().addTextures("Textures/NormalMap", "png|jpg");

    this->camera->setDepthField(10, 1320);
    this->camera->setView(Vec3(0, 0, 260), Vec3(0, 0, 0), Vec3(0, 1, 0));
    this->camera->setQueueCullFunc([](int queue) {
        if (queue == RenderQueues::UI) {
            return false;
        }
        return true;
    });

    Shader::ptr shader = ShaderManager::getInstance().GetResource("normal_map");
    {
        // set vertex attribute
        Attribute *vertAttr = shader->getAttribute("inVertex");
        vertAttr->setType(AttributeFormat::FLOAT);
        vertAttr->setSize(3);
        vertAttr->setStride(sizeof(NormalMapVertex));
        vertAttr->setOffset(0);

        Attribute *normalAttr = shader->getAttribute("inTexCoord");
        normalAttr->setType(AttributeFormat::FLOAT);
        normalAttr->setSize(3);
        normalAttr->setStride(sizeof(NormalMapVertex));
        normalAttr->setOffset((3) * 4);

        Attribute *uvAttr = shader->getAttribute("inNormal");
        uvAttr->setType(AttributeFormat::FLOAT);
        uvAttr->setSize(2);
        uvAttr->setStride(sizeof(NormalMapVertex));
        uvAttr->setOffset((5) * 4);

        Attribute *colorAttr = shader->getAttribute("inTangent");
        colorAttr->setType(AttributeFormat::FLOAT);
        colorAttr->setSize(4);
        colorAttr->setStride(sizeof(NormalMapVertex));
        colorAttr->setOffset((8) * 4);
    }

    auto texture = TextureManager::getInstance().getTexture("diffuse");

    auto geometry = ShapeGenerater::getInstance().CreateSphere(50, 30, 30);

    auto box = CreateMeshNode();
    SetMeshData(box, geometry, texture, "normal_map");
    auto renderElement = box->getComponent<RenderElement>();
    auto mesh = renderElement->getMesh();
    mesh->getGeometry()->calculateTangents();
    {
        auto meshData = Create<MeshData>();
        mesh->setMeshData(meshData);

        StreamUnit unit;
        unit.format.push_back(VertexElement(VertexElementType::Position, AttributeFormat::FLOAT, 3));
        unit.format.push_back(VertexElement(VertexElementType::TextureCoord, AttributeFormat::FLOAT, 2));
        unit.format.push_back(VertexElement(VertexElementType::Normal, AttributeFormat::FLOAT, 3));
        unit.format.push_back(VertexElement(VertexElementType::Tangent, AttributeFormat::FLOAT, 4));
        meshData->vertexStreams.push_back(unit);
    }
    auto func = [](Geometry &geometry, MeshData &meshData) {
        auto& stream = meshData.vertexStreams[0];

        stream.vertices.allocate(geometry.getVertexCount() * stream.getVertexSize());
        meshData.indices.allocate(geometry.faces.size() * 3);

        auto vertexPointer = Map<NormalMapVertex>(stream.vertices);
        auto facePointer = Map<Face>(meshData.indices);

        NormalMapVertex v;
        for (uint i = 0; i < geometry.getVertexCount(); i++) {
            v.xyz = geometry.positions[i];
            v.uv = geometry.uvs[i];
            v.normal = geometry.vertexNormals[i];
            v.tangent = geometry.tangents[i];

            vertexPointer[i] = v;
        }

        for (uint i = 0; i < geometry.faces.size(); i++) {
            facePointer[i] = geometry.faces[i];
        }
    };
    mesh->getGeometry()->setMeshDataAppendFunc(func);
    InitMeshInHardward(renderElement, "normal_map");

    {
        auto diffuseTexture = TextureManager::getInstance().getTexture("diffuse");
        auto normalTexture = TextureManager::getInstance().getTexture("diffusenormalmap");

        auto material = renderElement->getMaterial();
        material->setTexture("sBaseTex", diffuseTexture);
        material->setTexture("sNormalMap", normalTexture);
    }

    box->getTransform()->setLocalTranslation(Vec3(0, 0, 52));
    box->getTransform()->setLocalRotation(Quat().fromAngles(Vec3(50, 20, 0)));

    rootNode->addChild(box->getNode());
}

static Vec3 lightPos;
static float angle;
void Bumpmap::Update()
{
    float dt = GameHub::getInstance().GetGameTime().GetSecond();
    angle += dt * 60 * PI / 1000;
    lightPos.x = sin(angle) * 200;
    lightPos.y = cos(angle) * 200;
    lightPos.z = 0;

    auto shader = ShaderManager::getInstance().GetResource("normal_map");
    shader->getUniform("LightPosModel")->setData(lightPos);
}
