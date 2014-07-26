
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aColor;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;


#if MAX_SPOT_LIGHTS > 0 || defined( USE_BUMPMAP ) || defined( USE_ENVMAP )

    varying vec3 vWorldPosition;

#endif


void main()
{
    vec4 worldPosition = modelMatrix * vec4( aPosition, 1.0 );

#if MAX_SPOT_LIGHTS > 0 || defined( USE_BUMPMAP ) || defined( USE_ENVMAP )

    vWorldPosition = worldPosition.xyz;

#endif

}
