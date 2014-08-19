#include "ShapeGenerater.h"
#include "Texture/TextureManager.h"

ShapeGenerater::ShapeGenerater()
{
}

static void AppendVertexNormal(Geometry& geometry, std::map<uint, Vec3> normals) {
    for (auto pair : normals) {
        Vertex& v = geometry.getVertices()[pair.first];

        v.normal = pair.second;
    }
}

static void AppendVertexUv(Geometry& geometry, std::map<uint, Uv> uvs) {
    for (auto pair : uvs) {
        Vertex& v = geometry.getVertices()[pair.first];

        v.uv = pair.second;
    }
}

GeometryPtr ShapeGenerater::CreateBox(float width, float height, float depth, int widthSegments, int heightSegments, int depthSegments)
{
    GeometryPtr p = Geometry::create();
    Geometry& geometry = *(p);

    float width_half = width / 2;
    float height_half = height / 2;
    float depth_half = depth / 2;

    auto buildPlane = [&](char u, char v, int udir, int vdir, float width, float height, float depth, int materialIndex) {

        int gridX = widthSegments;
        int gridY = heightSegments;

        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;

        int xIndex, yIndex, zIndex;
        if ((u == 'x' && v == 'y')) {
            xIndex = 0;
            yIndex = 1;
            zIndex = 2;
        } else if (u == 'y' && v == 'x') {
            xIndex = 1;
            yIndex = 0;
            zIndex = 2;
        } else if ((u == 'x' && v == 'z')) {
            xIndex = 0;
            yIndex = 2;
            zIndex = 1;

            gridY = depthSegments;
        } else if ((u == 'z' && v == 'x')) {
            xIndex = 2;
            yIndex = 0;
            zIndex = 1;

            gridY = depthSegments;
        } else if ((u == 'y' && v == 'z')) {
            xIndex = 1;
            yIndex = 2;
            zIndex = 0;

            gridX = depthSegments;
        } else if ((u == 'z' && v == 'y')) {
            xIndex = 2;
            yIndex = 1;
            zIndex = 0;

            gridX = depthSegments;
        } else {
            assert(false);
        }

        int vertexCountX = gridX + 1;
        int vertexCountY = gridY + 1;

        float segmentWidth = width / gridX;
        float segmentHeight = height / gridY;

        int offset = geometry.getVertices().size();

        for (int iy = 0; iy < vertexCountY; ++iy) {
            for (int ix = 0; ix < vertexCountX; ++ix) {
                float x = ix * segmentWidth - halfWidth;
                float y = iy * segmentHeight - halfHeight;

                Vertex v;
                v.xyz[xIndex] = x * udir;
                v.xyz[yIndex] = y * vdir;
                v.xyz[zIndex] = depth;
                geometry.addVertex(v);
            }
        }

        Vec3 normal(0, 0, 0);
        normal[zIndex] = depth > 0 ? 1 : -1;

        for (int iy = 0; iy < gridY; ++iy) {
            for (int ix = 0; ix < gridX; ++ix) {

                /*
                 *  a --- d
                 *  |   / |
                 *  | /   |
                 *  b --- c
                 */

                int a = ix + vertexCountX * iy + offset;
                int b = ix + vertexCountX * ( iy + 1 ) + offset;
                int c = ( ix + 1 ) + vertexCountX * ( iy + 1 ) + offset;
                int d = ( ix + 1 ) + vertexCountX * iy + offset;

                Uv uva = {ix/(float)gridX, 1 - iy/(float)gridY};
                Uv uvb = {ix/(float)gridX, 1 - (iy + 1)/(float)gridY};
                Uv uvc = {(ix + 1)/(float)gridX, 1 - (iy + 1)/(float)gridY};
                Uv uvd = {(ix + 1)/(float)gridX, 1 - iy/(float)gridY};

                std::map<uint, Vec3> normals = {{a, normal},
                                               {b, normal},
                                               {c, normal},
                                               {d, normal}};

                std::map<uint, Uv> uvs = {{a, uva},
                                         {b, uvb},
                                         {c, uvc},
                                         {d, uvd}};

                AppendVertexNormal(geometry, normals);
                AppendVertexUv(geometry, uvs);

                Face face1(a, b, d);
                Face face2(b, c, d);

                geometry.addFace(face1);
                geometry.addFace(face2);
            }
        }
    };

    buildPlane( 'z', 'y', - 1, - 1, depth, height, width_half, 0 ); // px
    buildPlane( 'z', 'y',   1, - 1, depth, height, - width_half, 1 ); // nx
    buildPlane( 'x', 'z',   1,   1, width, depth, height_half, 2 ); // py
    buildPlane( 'x', 'z',   1, - 1, width, depth, - height_half, 3 ); // ny
    buildPlane( 'x', 'y',   1, - 1, width, height, depth_half, 4 ); // pz
    buildPlane( 'x', 'y', - 1, - 1, width, height, - depth_half, 5 ); // nz

    return p;
}

GeometryPtr ShapeGenerater::CreatePlane(float width, float height, int widthSegments, int heightSegments)
{
    GeometryPtr p = Geometry::create();
    Geometry& geometry = *(p);

    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    int gridX = widthSegments;
    int gridY = heightSegments;

    float vertexCountX = gridX + 1;
    float vertexCountY = gridY + 1;

    float segmentWidth = width / gridX;
    float segmentHeight = height / gridY;

    for (int iy = 0; iy < vertexCountY; ++iy) {

        float y = iy * segmentHeight - halfHeight;

        for (int ix = 0; ix < vertexCountX; ++ix) {
            float x = ix * segmentWidth - halfWidth;
            Vertex v;
            // Z is up, so reverse Y
            v.xyz.set(x, -y, 0);
            geometry.addVertex(v);
        }
    }

    Vec3 normal(0, 0, 1);
    for (int iy = 0; iy < heightSegments; ++iy) {
        for (int ix = 0; ix < widthSegments; ++ix) {

            /*
             *  a --- d
             *  |   / |
             *  | /   |
             *  b --- c
             */

            int a = ix + vertexCountX * iy;
            int b = ix + vertexCountX * ( iy + 1 );
            int c = ( ix + 1 ) + vertexCountX * ( iy + 1 );
            int d = ( ix + 1 ) + vertexCountX * iy;

            Uv uva = {ix/(float)gridX, 1 - iy/(float)gridY};
            Uv uvb = {ix/(float)gridX, 1 - (iy + 1)/(float)gridY};
            Uv uvc = {(ix + 1)/(float)gridX, 1 - (iy + 1)/(float)gridY};
            Uv uvd = {(ix + 1)/(float)gridX, 1 - iy/(float)gridY};

            std::map<uint, Vec3> normals = {{a, normal},
                                           {b, normal},
                                           {c, normal},
                                           {d, normal}};

            std::map<uint, Uv> uvs = {{a, uva},
                                     {b, uvb},
                                     {c, uvc},
                                     {d, uvd}};

            AppendVertexNormal(geometry, normals);
            AppendVertexUv(geometry, uvs);

            Face face1(a, b, d);
            Face face2(b, c, d);

            geometry.addFace(face1);
            geometry.addFace(face2);
        }
    }

    return p;
}

GeometryPtr ShapeGenerater::CreateCylinder(float radiusTop, float radiusBottom, float height, int radialSegments, int heightSegments, bool openEnded)
{
    GeometryPtr p = Geometry::create();
    Geometry& geometry = *(p);

    float halfHeight = height / 2.0f;

    std::vector<std::vector<uint>> vertexIndexes;

    for (int iz = 0; iz < heightSegments + 1; ++iz) {

        float v = iz / (float)heightSegments;
        float radius = v * (radiusTop - radiusBottom) + radiusBottom;

        std::vector<uint> vertexRow;

        for (int ir = 0; ir < radialSegments + 1; ++ir) {

            float u = ir / (float)radialSegments;

            Vertex vertex;
            vertex.xyz.x = radius * cos(u * PI * 2);
            vertex.xyz.y = radius * sin(u * PI * 2);
            vertex.xyz.z = v * height - halfHeight;

            vertex.uv.u = u;
            vertex.uv.v = 1 - v;

            geometry.addVertex(vertex);

            vertexRow.push_back(geometry.getVertices().size() - 1);
        }

        vertexIndexes.push_back(vertexRow);
    }

    float tanTheta = ( radiusBottom - radiusTop ) / height;

    Vec3 na, nb;
    for (int ir = 0; ir < radialSegments; ++ir) {

        uint nai, nbi;
        if (radiusBottom != 0) {
            nai = vertexIndexes[0][ir];
            nbi = vertexIndexes[0][ir + 1];
        } else {
            nai = vertexIndexes[1][ir];
            nbi = vertexIndexes[1][ir + 1];
        }

        na = geometry.getVertices()[nai].xyz;
        nb = geometry.getVertices()[nbi].xyz;

        na.z = sqrt(na.x * na.x + na.y * na.y) * tanTheta;
        na.normalizeSelf();

        nb.z = sqrt(nb.x * nb.x + nb.y * nb.y) * tanTheta;
        nb.normalizeSelf();

        for (int iz = 0; iz < heightSegments; ++iz) {

            uint a = vertexIndexes[iz + 1][ir];
            uint b = vertexIndexes[iz][ir];
            uint c = vertexIndexes[iz][ir + 1];
            uint d = vertexIndexes[iz + 1][ir + 1];

            geometry.addFace(Face(a, b, d));
            geometry.addFace(Face(b, c, d));


            std::map<uint, Vec3> normals = {{a, na},
                                           {b, na},
                                           {c, nb},
                                           {d, nb}};

            AppendVertexNormal(geometry, normals);
        }
    }

    // top cap
    if (!openEnded && radiusTop > 0) {
        Vertex center;
        center.xyz = Vec3(0, 0, halfHeight);
        center.uv = Uv(1, 1);
        geometry.addVertex(center);

        for (int ir = 0; ir < radialSegments; ++ir) {
            int rowIndex = vertexIndexes.size() - 1;
            uint a = vertexIndexes[rowIndex][ir];
            uint b = vertexIndexes[rowIndex][ir + 1];
            uint c = geometry.getVertices().size() - 1;

            // TODO: uv of face?

            Vec3 normal(0, 0, 1);
            std::map<uint, Vec3> normals = {{a, normal},
                                           {b, normal},
                                           {c, normal}};

            AppendVertexNormal(geometry, normals);

            geometry.addFace(Face(a, b, c));
        }
    }

    // bottom cap
    if (!openEnded && radiusBottom > 0) {
        Vertex center;
        center.xyz = Vec3(0, 0, -halfHeight);
        center.uv = Uv(0, 0);
        geometry.addVertex(center);

        for (int ir = 0; ir < radialSegments; ++ir) {
            uint a = vertexIndexes[0][ir + 1];
            uint b = vertexIndexes[0][ir];
            uint c = geometry.getVertices().size() - 1;

            // TODO: uv of face?

            Vec3 normal(0, 0, -1);
            std::map<uint, Vec3> normals = {{a, normal},
                                           {b, normal},
                                           {c, normal}};

            AppendVertexNormal(geometry, normals);

            geometry.addFace(Face(a, b, c));
        }
    }

    return p;
}

GeometryPtr ShapeGenerater::CreateRing(float innerRadius, float outerRadius, int thetaSegments, int phiSegments, float thetaStart, float thetaLength)
{
    GeometryPtr p = Geometry::create();
    Geometry& geometry = *(p);

    float radius = innerRadius;
    float radiusStep = (outerRadius - innerRadius) / phiSegments;
    for (int pi=0; pi < phiSegments + 1; ++pi) {
        for (int ti=0; ti < thetaSegments + 1; ++ti) {

            float theta = thetaStart + ti / (float)thetaSegments * thetaLength;

            Vertex vertex;
            vertex.xyz.x = radius * cos(theta);
            vertex.xyz.y = radius * sin(theta);

            vertex.uv.u = (vertex.xyz.x / outerRadius + 1) / 2.0f;
            vertex.uv.v = (vertex.xyz.y / outerRadius + 1) / 2.0f;

            geometry.addVertex(vertex);
        }

        radius += radiusStep;
    }

    for (int ps=0; ps < phiSegments; ++ps) {
        for (int ts=0; ts < thetaSegments; ++ts) {

            uint a = (thetaSegments + 1) * ps + ts + 1;
            uint c = (thetaSegments + 1) * (ps + 1) + ts;
            uint b = (thetaSegments + 1) * ps + ts;
            uint d = (thetaSegments + 1) * (ps + 1) + ts + 1;

            geometry.addFace(Face(a, b, d));
            geometry.addFace(Face(b, c, d));
        }
    }

    return p;
}

GeometryPtr ShapeGenerater::CreateTorus(float radius, float tubularRadius, int radialSegments, int tubularSegments, float tubularStart, float tubularLength)
{
    GeometryPtr p = Geometry::create();
    Geometry& geometry = *(p);

    Vec3 center;
    for (int ri=0; ri < radialSegments + 1; ++ri) {
        for (int ti=0; ti < tubularSegments + 1; ++ti) {
            float u = ti / (float)tubularSegments * tubularLength + tubularStart;
            float v = ri / (float)radialSegments * PI * 2;

            center.x = radius * cos(u);
            center.y = radius * sin(u);

            Vertex vertex;
            vertex.xyz.x = (radius + tubularRadius * cos(v)) * cos(u);
            vertex.xyz.y = (radius + tubularRadius * cos(v)) * sin(u);
            vertex.xyz.z = tubularRadius * sin(v);

            vertex.uv.u = ti / (float)tubularSegments;
            vertex.uv.v = ri / (float)radialSegments;

            vertex.normal = (vertex.xyz - center).normalize();

            geometry.addVertex(vertex);
        }
    }

    for (int rs=0; rs < radialSegments; ++rs) {
        for (int ts=0; ts < tubularSegments; ++ts) {

            uint a = ( tubularSegments + 1 ) * (rs + 1) + ts;
            uint b = ( tubularSegments + 1 ) * rs + ts;
            uint c = ( tubularSegments + 1 ) * rs + ts + 1;
            uint d = ( tubularSegments + 1 ) * (rs + 1) + ts + 1;

            geometry.addFace(Face(a, b, d));
            geometry.addFace(Face(b, c, d));
        }
    }

    return p;
}

GeometryPtr ShapeGenerater::CreateSphere(float radius, int widthSegments, int heightSegments, float phiStart, float phiLength, float thetaStart, float thetaLenght)
{
    GeometryPtr p = Geometry::create();
    Geometry& geometry = *(p);

    std::vector<std::vector<uint>> vertexIndexes;
    for (int iy = 0; iy < heightSegments + 1; ++iy) {

        std::vector<uint> vertexRow;
        for (int ix = 0; ix < widthSegments + 1; ++ix) {

            float u = ix / (float)widthSegments;
            float v = iy / (float)heightSegments;

            Vertex vertex;
            vertex.xyz.x = -radius * sin(thetaStart + v * thetaLenght) * cos(phiStart + u * phiLength);
            vertex.xyz.y = radius * cos(thetaStart + v * thetaLenght);
            vertex.xyz.z = radius * sin(thetaStart + v * thetaLenght) * sin(phiStart + u * phiLength);

            vertex.uv.set(u, 1-v);

            geometry.addVertex(vertex);

            vertexRow.push_back(geometry.getVertices().size() - 1);
        }

        vertexIndexes.push_back(vertexRow);
    }

    for (int sy=0; sy < heightSegments; ++sy) {
        for (int sx=0; sx < widthSegments; ++sx) {

            uint a = vertexIndexes[sy][sx + 1];
            uint b = vertexIndexes[sy][sx];
            uint c = vertexIndexes[sy + 1][sx];
            uint d = vertexIndexes[sy + 1][sx + 1];

            Vec3 na = geometry.getVertices()[a].xyz.normalize();
            Vec3 nb = geometry.getVertices()[b].xyz.normalize();
            Vec3 nc = geometry.getVertices()[c].xyz.normalize();
            Vec3 nd = geometry.getVertices()[d].xyz.normalize();

            std::map<uint, Vec3> normals = {{a, na},
                                           {b, nb},
                                           {c, nc},
                                           {d, nd}};
            AppendVertexNormal(geometry, normals);

            if (abs(geometry.getVertices()[a].xyz.y) == radius) {
                geometry.addFace(Face(a, c, d));
            } else if (abs(geometry.getVertices()[c].xyz.y) == radius) {
                geometry.addFace(Face(a, b, c));
            } else {
                geometry.addFace(Face(a, b, d));
                geometry.addFace(Face(b, c, d));
            }
        }
    }

    return p;
}

MeshPtr ShapeGenerater::CreateBox(float side, Texture::ptr texture)
{
    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->init();

    GeometryPtr geometry;
    geometry = this->CreateBox(side, side, side, 10, 10, 10);
//    geometry = this->CreateTorus(40, 20, 40, 40);
    mesh->setGeometry(geometry);

    TextureUnitState::ptr unit = mesh->getMaterial()->getPass(0)->getTextureUnit(0);
    unit->setUVstate(0, 0, 1, 1, 0);

    if (texture == nullptr) {
        texture = TextureManager::getInstance().getTexture("girl");
    }
    unit->addTextureFrame(texture);

    return mesh;
}

MeshPtr ShapeGenerater::CreateMesh(GeometryPtr& geometry, MaterialPtr& material)
{
    MeshPtr mesh = std::make_shared<Mesh>();

    mesh->setGeometry(geometry);
    mesh->setMaterial(material);

    return mesh;
}

MeshPtr ShapeGenerater::CreateMesh(GeometryPtr &geometry, Texture::ptr texture)
{
    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->init();

    mesh->setGeometry(geometry);

    TextureUnitState::ptr unit = mesh->getMaterial()->getPass(0)->getTextureUnit(0);
    unit->setUVstate(0, 0, 1, 1, 0);

    if (texture == nullptr) {
        texture = TextureManager::getInstance().getTexture("girl");
    }
    unit->addTextureFrame(texture);

    return mesh;
}
