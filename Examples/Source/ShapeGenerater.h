#ifndef SHAPEGENERATER_H
#define SHAPEGENERATER_H

#include "Scene/Mesh.h"
#include "Math/Math.h"
#include "Base/Singleton.h"

using namespace re;

class ShapeGenerater : public Singleton<ShapeGenerater>
{
public:
    ShapeGenerater();

    GeometryPtr CreateBox(float width, float height, float depth, int widthSegments=1, int heightSegments=1, int depthSegments=1);
    GeometryPtr CreatePlane(float width, float height, int widthSegments=1, int heightSegments=1);
    GeometryPtr CreateCylinder(float radiusTop, float radiusBottom, float height, int radialSegments=8, int heightSegments=1, bool openEnded=false);
    GeometryPtr CreateRing(float innerRadius, float outerRadius, int thetaSegments=8, int phiSegments=8, float thetaStart=0, float thetaLength=PI* 2);
    GeometryPtr CreateTorus(float radius, float tubularRadius, int radialSegments=8, int tubularSegments=6, float tubularStart=0, float tubularLength=PI* 2);
    GeometryPtr CreateSphere(float radius, int widthSegments=8, int heightSegments=6, float phiStart=0, float phiLength=PI*2, float thetaStart=0, float thetaLenght=PI*2);
};

#endif // SHAPEGENERATER_H
