#ifndef PREDEFINETWEENACCESSOR_H
#define PREDEFINETWEENACCESSOR_H

namespace re {

namespace FlatNodeAccessor {
    const int POSITION_X = 1;
    const int POSITION_Y = 2;
    const int POSITION_XY = 3;
    const int SCALE_X = 4;
    const int SCALE_Y = 5;
    const int SCALE_XY = 6;
    const int ANCHOR_X = 7;
    const int ANCHOR_Y = 8;
    const int ANCHOR_XY = 9;
    const int ROTATION = 10;
    const int COLOR = 11;
    const int ALPHA = 12;
    const int OPACITY = 13;
}

class PredefineTweenAccessor
{
public:
    static void registerAccessor();
};

}

#endif // PREDEFINETWEENACCESSOR_H
