#ifndef SHAPEGENERATER_H
#define SHAPEGENERATER_H

#include "Scene/Mesh.h"

#include "Base/Singleton.h"

using namespace re;

class ShapeGenerater : public Singleton<ShapeGenerater>
{
public:
    ShapeGenerater();

    MeshPtr CreateBox(float side, Texture::ptr texture = nullptr);
};

#endif // SHAPEGENERATER_H
