#ifndef CONTROLPOINTS_H
#define CONTROLPOINTS_H

#include <vector>
#include <array>

#include "platform.h"
#include "Vertex.h"

namespace re {

class Geometry;

class ControlPoints
{
    friend class SkeletonController;
    friend class FbxParser;

public:
    ControlPoints() {};

protected:
    void cacheVertex();

protected:
    std::vector<Vec3> controlPoints;
    std::vector<int> vertexToControl;
    std::vector<vector<int>> controlToVertex;
};

}

#endif // CONTROLPOINTS_H
