#include "ShapeGenerater.h"
#include "Texture/TextureManager.h"

ShapeGenerater::ShapeGenerater()
{
}

MeshPtr ShapeGenerater::CreateBox(float side, Texture::ptr texture)
{
    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->init();

    float coords[] = {
        -side/2.0f, -side/2.0f, side/2.0f, //v0
        side/2.0f, -side/2.0f, side/2.0f, 	//v1
        -side/2.0f, side/2.0f, side/2.0f, 	//v2
        side/2.0f, side/2.0f, side/2.0f, 	//v3

        side/2.0f, -side/2.0f, side/2.0f, 	//...
        side/2.0f, -side/2.0f, -side/2.0f,
        side/2.0f, side/2.0f, side/2.0f,
        side/2.0f, side/2.0f, -side/2.0f,

        side/2.0f, -side/2.0f, -side/2.0f,
        -side/2.0f, -side/2.0f, -side/2.0f,
        side/2.0f, side/2.0f, -side/2.0f,
        -side/2.0f, side/2.0f, -side/2.0f,

        -side/2.0f, -side/2.0f, -side/2.0f,
        -side/2.0f, -side/2.0f, side/2.0f,
        -side/2.0f, side/2.0f, -side/2.0f,
        -side/2.0f, side/2.0f, side/2.0f,

        -side/2.0f, -side/2.0f, -side/2.0f,
        side/2.0f, -side/2.0f, -side/2.0f,
        -side/2.0f, -side/2.0f, side/2.0f,
        side/2.0f, -side/2.0f, side/2.0f,

        -side/2.0f, side/2.0f, side/2.0f,
        side/2.0f, side/2.0f, side/2.0f,
        -side/2.0f, side/2.0f, -side/2.0f,
        side/2.0f, side/2.0f, -side/2.0f,
            };

    float textureCood[] = {
        //Mapping coordinates for the vertices
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };

    float normals[] = {
                        //Normals
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,

                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,

                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,

                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,

                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,

                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f
                                            };

    short indices[] = {
        // Faces definition
        0, 1, 3, 0, 3, 2, 		// Face front
        4, 5, 7, 4, 7, 6, 		// Face right
        8, 9, 11, 8, 11, 10, 	// ...
        12, 13, 15, 12, 15, 14,
        16, 17, 19, 16, 19, 18,
        20, 21, 23, 20, 23, 22,
    };

    for (int i = 0; i < 24; ++i) {
      Vertex v;
      v.xyz.set(coords[i * 3 + 0], coords[i * 3 + 1], coords[i * 3 + 2]);

      v.uv.set(textureCood[i * 2 + 0], textureCood[i * 2 + 1]);

      v.normal.set(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);

      mesh->getGeometry()->addVertex(v);
    }

    for (int i = 0; i < 12; ++i) {
        Face f(indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]);

        mesh->getGeometry()->addFace(f);
    }

    TextureUnitState &unit = mesh->getMaterial()->getTexture();
    unit.setUVstate(0, 0, 1, 1, 0);

    if (texture == nullptr) {
        texture = TextureManager::getInstance().getTexture("girl");
    }
    unit.addTextureFrame(texture);

    return mesh;
}
