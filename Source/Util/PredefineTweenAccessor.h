#ifndef PREDEFINETWEENACCESSOR_H
#define PREDEFINETWEENACCESSOR_H

namespace re {

namespace FlatNodeAccessor {
enum {
    POSITION_X,
    POSITION_Y,
    POSITION_XY,
    SCALE,
    SCALE_X,
    SCALE_Y,
    SCALE_XY,
    ANCHOR_X,
    ANCHOR_Y,
    ANCHOR_XY,
    ROTATION,
    COLOR,
    ALPHA,
    OPACITY,
};
};

class PredefineTweenAccessor
{
public:
    static void registerAccessor();
};

}

#endif // PREDEFINETWEENACCESSOR_H
