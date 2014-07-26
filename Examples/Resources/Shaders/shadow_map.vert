#define USE_SHADOWMAP
#define MAX_SHADOWS 2

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aColor;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;

#ifdef USE_SHADOWMAP

    varying vec4 vShadowCoord[ MAX_SHADOWS ];
    uniform mat4 shadowMatrix[ MAX_SHADOWS ];

#endif

void main()
{

    vTexCoord = aTexCoord;
    vFragmentColor = aColor;

    gl_Position = projection*(view*(model*vec4(aPosition,1.0)));

#ifdef USE_SHADOWMAP
    vec4 worldPosition = model * vec4( aPosition, 1.0 );

    for( int i = 0; i < MAX_SHADOWS; i ++ ) {

        vShadowCoord[ i ] = shadowMatrix[ i ] * worldPosition;

    }

#endif
}
