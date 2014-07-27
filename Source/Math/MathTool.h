#ifndef MATHTOOL_H
#define MATHTOOL_H

const static float  PI				= 3.14159265358979323846f;
const static float	TWO_PI			= 2.0f * PI;
const static float	HALF_PI			= 0.5f * PI;
const static float	ONEFOURTH_PI	= 0.25f * PI;
const static float  E				= 2.71828182845904523536f;
const static float  DEG_TO_RAD      = PI / 180.0f;
const static float  RAD_TO_DEG      = 180.0f / PI;

inline float DegreeToRadian (const float d) {
    return d * DEG_TO_RAD;
}

inline float RadianToDegree(const float r) {
    return r * RAD_TO_DEG;
}

inline float Fabs( float f ) {
    int tmp = *reinterpret_cast<int *>( &f );
    tmp &= 0x7FFFFFFF;
    return *reinterpret_cast<float *>( &tmp );
}

inline int CeilPowerOfTwo( int x ) {
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

inline int FloorPowerOfTwo( int x ) {
    return CeilPowerOfTwo( x ) >> 1;
}

inline bool IsPowerOfTwo( int x ) {
    return ( x & ( x - 1 ) ) == 0 && x > 0;
}

#endif // MATHTOOL_H
