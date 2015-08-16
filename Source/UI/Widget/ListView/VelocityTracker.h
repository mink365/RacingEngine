#ifndef VELOCITYTRACKER_H
#define VELOCITYTRACKER_H

#include <array>
#include <Math/Vector.h>

namespace re {

class VelocityTracker
{
public:
    void addPoint(const Vec2& p, int64_t time);
    void computeVelocity();

    const Vec2& getVelocity();

private:
    int poolIndexToArrayIndex(int begin, int index);

private:
    static const int NUM_PAST = 10;
    static const int64_t LONGEST_PAST_TIME = 200;
    std::array<Vec2, NUM_PAST> _pastPoint;
    std::array<int64_t, NUM_PAST> _pastTime;

    Vec2 _velocity;

    int _begine, _index;
};

}

#endif // VELOCITYTRACKER_H
