#ifndef MATHTOOL_H
#define MATHTOOL_H

const static float  PI				= 3.14159265358979323846f;
const static float	TWO_PI			= 2.0f * PI;
const static float	HALF_PI			= 0.5f * PI;
const static float	ONEFOURTH_PI	= 0.25f * PI;
const static float  E				= 2.71828182845904523536f;
const static float  DEG_TO_RAD      = PI / 180.0f;
const static float  RAD_TO_DEG      = 180.0f / PI;

inline float degreeToRadian (const float d) {
    return d * DEG_TO_RAD;
}

inline float radianToDegree(const float r) {
    return r * RAD_TO_DEG;
}

inline float Fabs( float f ) {
    int tmp = *reinterpret_cast<int *>( &f );
    tmp &= 0x7FFFFFFF;
    return *reinterpret_cast<float *>( &tmp );
}

#endif // MATHTOOL_H
