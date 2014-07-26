#define MAX_DIR_LIGHTS 0
#define MAX_HEMI_LIGHTS 0
#define MAX_POINT_LIGHTS 2
#define MAX_SPOT_LIGHTS 0


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec3 aNormal;
attribute vec4 aColor;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;


#if MAX_SPOT_LIGHTS > 0 || defined( USE_BUMPMAP ) || defined( USE_ENVMAP )

    varying vec3 vWorldPosition;

#endif

varying vec3 vViewPosition;
varying vec3 vNormal;

void main()
{
    vec4 worldPosition = modelMatrix * vec4( aPosition, 1.0 );

#if MAX_SPOT_LIGHTS > 0 || defined( USE_BUMPMAP ) || defined( USE_ENVMAP )

    vWorldPosition = worldPosition.xyz;

#endif

    // ------------------- normal ----------------------

    vec3 objectNormal;

    #ifdef USE_SKINNING

        objectNormal = skinnedNormal.xyz;

    #endif

    #if !defined( USE_SKINNING ) && defined( USE_MORPHNORMALS )

        objectNormal = morphedNormal;

    #endif

    #if !defined( USE_SKINNING ) && ! defined( USE_MORPHNORMALS )

        objectNormal = aNormal;

    #endif

    #ifdef FLIP_SIDED

        objectNormal = -objectNormal;

    #endif

    mat4 modelViewMatrix = viewMatrix * modelMatrix;

    // mat3 normalMatrix = mat3(modelViewMatrix[0].xyz, modelViewMatrix[1].xyz, modelViewMatrix[2].xyz);
    vec3 transformedNormal = normalMatrix * objectNormal;

    vec4 mvPosition;

    #ifdef USE_SKINNING

        mvPosition = modelViewMatrix * skinned;

    #endif

    #if !defined( USE_SKINNING ) && defined( USE_MORPHTARGETS )

        mvPosition = modelViewMatrix * vec4( morphed, 1.0 );

    #endif

    #if !defined( USE_SKINNING ) && ! defined( USE_MORPHTARGETS )

        mvPosition = modelViewMatrix * vec4( aPosition, 1.0 );

    #endif

    vNormal = normalize( transformedNormal );
    vViewPosition = -mvPosition.xyz;

    vTexCoord = aTexCoord;
    vFragmentColor = aColor;
    gl_Position = projectionMatrix * mvPosition;

}
