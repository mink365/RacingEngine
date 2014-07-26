
#define MAX_DIR_LIGHTS 3
#define MAX_HEMI_LIGHTS 3
#define MAX_POINT_LIGHTS 3
#define MAX_SPOT_LIGHTS 3


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aColor;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;

// ---------------- light attr -------------------------------------

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 emissive;

uniform vec3 ambientLightColor;

#if MAX_DIR_LIGHTS > 0

    uniform vec3 directionalLightColor[ MAX_DIR_LIGHTS ];
    uniform vec3 directionalLightDirection[ MAX_DIR_LIGHTS ];

#endif

#if MAX_HEMI_LIGHTS > 0

    uniform vec3 hemisphereLightSkyColor[ MAX_HEMI_LIGHTS ];
    uniform vec3 hemisphereLightGroundColor[ MAX_HEMI_LIGHTS ];
    uniform vec3 hemisphereLightDirection[ MAX_HEMI_LIGHTS ];

#endif

#if MAX_POINT_LIGHTS > 0

    uniform vec3 pointLightColor[ MAX_POINT_LIGHTS ];
    uniform vec3 pointLightPosition[ MAX_POINT_LIGHTS ];
    uniform float pointLightDistance[ MAX_POINT_LIGHTS ];

#endif

#if MAX_SPOT_LIGHTS > 0

    uniform vec3 spotLightColor[ MAX_SPOT_LIGHTS ];
    uniform vec3 spotLightPosition[ MAX_SPOT_LIGHTS ];
    uniform vec3 spotLightDirection[ MAX_SPOT_LIGHTS ];
    uniform float spotLightDistance[ MAX_SPOT_LIGHTS ];
    uniform float spotLightAngleCos[ MAX_SPOT_LIGHTS ];
    uniform float spotLightExponent[ MAX_SPOT_LIGHTS ];

#endif

#ifdef WRAP_AROUND

    uniform vec3 wrapRGB;

#endif


void main()
{

    vec4 worldPosition = modelMatrix * vec4( aPosition, 1.0 );

    // ------------------- normal ----------------------

    vec3 objectNormal;

    #ifdef USE_SKINNING

        objectNormal = skinnedNormal.xyz;

    #endif

    #if !defined( USE_SKINNING ) && defined( USE_MORPHNORMALS )

        objectNormal = morphedNormal;

    #endif

    #if !defined( USE_SKINNING ) && ! defined( USE_MORPHNORMALS )

        objectNormal = normal;

    #endif

    #ifdef FLIP_SIDED

        objectNormal = -objectNormal;

    #endif

    vec3 transformedNormal = normalMatrix * objectNormal;

    vec4 mvPosition;

    #ifdef USE_SKINNING

        mvPosition = modelViewMatrix * skinned;

    #endif

    #if !defined( USE_SKINNING ) && defined( USE_MORPHTARGETS )

        mvPosition = modelViewMatrix * vec4( morphed, 1.0 );

    #endif

    #if !defined( USE_SKINNING ) && ! defined( USE_MORPHTARGETS )

        mvPosition = modelViewMatrix * vec4( position, 1.0 );

    #endif

    gl_Position = projectionMatrix * mvPosition;

    // -------------------------- light func ---------------------------


    vLightFront = vec3( 0.0 );

    #ifdef DOUBLE_SIDED

        vLightBack = vec3( 0.0 );

    #endif

    transformedNormal = normalize( transformedNormal );

    #if MAX_DIR_LIGHTS > 0

    for( int i = 0; i < MAX_DIR_LIGHTS; i ++ ) {

        vec4 lDirection = viewMatrix * vec4( directionalLightDirection[ i ], 0.0 );
        vec3 dirVector = normalize( lDirection.xyz );

        float dotProduct = dot( transformedNormal, dirVector );
        vec3 directionalLightWeighting = vec3( max( dotProduct, 0.0 ) );

        #ifdef DOUBLE_SIDED

            vec3 directionalLightWeightingBack = vec3( max( -dotProduct, 0.0 ) );

            #ifdef WRAP_AROUND

                vec3 directionalLightWeightingHalfBack = vec3( max( -0.5 * dotProduct + 0.5, 0.0 ) );

            #endif

        #endif

        #ifdef WRAP_AROUND

            vec3 directionalLightWeightingHalf = vec3( max( 0.5 * dotProduct + 0.5, 0.0 ) );
            directionalLightWeighting = mix( directionalLightWeighting, directionalLightWeightingHalf, wrapRGB );

            #ifdef DOUBLE_SIDED

                directionalLightWeightingBack = mix( directionalLightWeightingBack, directionalLightWeightingHalfBack, wrapRGB );

            #endif

        #endif

        vLightFront += directionalLightColor[ i ] * directionalLightWeighting;

        #ifdef DOUBLE_SIDED

            vLightBack += directionalLightColor[ i ] * directionalLightWeightingBack;

        #endif

    }

    #endif

    #if MAX_POINT_LIGHTS > 0

    for( int i = 0; i < MAX_POINT_LIGHTS; i ++ ) {

        vec4 lPosition = viewMatrix * vec4( pointLightPosition[ i ], 1.0 );
        vec3 lVector = lPosition.xyz - mvPosition.xyz;

        float lDistance = 1.0;
        if ( pointLightDistance[ i ] > 0.0 )
            lDistance = 1.0 - min( ( length( lVector ) / pointLightDistance[ i ] ), 1.0 );

        lVector = normalize( lVector );
        float dotProduct = dot( transformedNormal, lVector );

        vec3 pointLightWeighting = vec3( max( dotProduct, 0.0 ) );

        #ifdef DOUBLE_SIDED

            vec3 pointLightWeightingBack = vec3( max( -dotProduct, 0.0 ) );

            #ifdef WRAP_AROUND

                vec3 pointLightWeightingHalfBack = vec3( max( -0.5 * dotProduct + 0.5, 0.0 ) );

            #endif

        #endif

        #ifdef WRAP_AROUND

            vec3 pointLightWeightingHalf = vec3( max( 0.5 * dotProduct + 0.5, 0.0 ) );
            pointLightWeighting = mix( pointLightWeighting, pointLightWeightingHalf, wrapRGB );

            #ifdef DOUBLE_SIDED

                pointLightWeightingBack = mix( pointLightWeightingBack, pointLightWeightingHalfBack, wrapRGB );

            #endif

        #endif

        vLightFront += pointLightColor[ i ] * pointLightWeighting * lDistance;

        #ifdef DOUBLE_SIDED

             vLightBack += pointLightColor[ i ] * pointLightWeightingBack * lDistance;

        #endif

    }
    #endif

    #if MAX_SPOT_LIGHTS > 0

    for( int i = 0; i < MAX_SPOT_LIGHTS; i ++ ) {

        vec4 lPosition = viewMatrix * vec4( spotLightPosition[ i ], 1.0 );
        vec3 lVector = lPosition.xyz - mvPosition.xyz;

        float spotEffect = dot( spotLightDirection[ i ], normalize( spotLightPosition[ i ] - worldPosition.xyz ) );

        if ( spotEffect > spotLightAngleCos[ i ] ) {

            spotEffect = max( pow( max( spotEffect, 0.0 ), spotLightExponent[ i ] ), 0.0 );

            float lDistance = 1.0;
            if ( spotLightDistance[ i ] > 0.0 )
                lDistance = 1.0 - min( ( length( lVector ) / spotLightDistance[ i ] ), 1.0 );

            lVector = normalize( lVector );

            float dotProduct = dot( transformedNormal, lVector );
            vec3 spotLightWeighting = vec3( max( dotProduct, 0.0 ) );

            #ifdef DOUBLE_SIDED

                vec3 spotLightWeightingBack = vec3( max( -dotProduct, 0.0 ) );

                #ifdef WRAP_AROUND

                    vec3 spotLightWeightingHalfBack = vec3( max( -0.5 * dotProduct + 0.5, 0.0 ) );

                #endif

            #endif

            #ifdef WRAP_AROUND

                vec3 spotLightWeightingHalf = vec3( max( 0.5 * dotProduct + 0.5, 0.0 ) );
                spotLightWeighting = mix( spotLightWeighting, spotLightWeightingHalf, wrapRGB );

                #ifdef DOUBLE_SIDED

                    spotLightWeightingBack = mix( spotLightWeightingBack, spotLightWeightingHalfBack, wrapRGB );

                #endif

            #endif

            vLightFront += spotLightColor[ i ] * spotLightWeighting * lDistance * spotEffect;

            #ifdef DOUBLE_SIDED

                vLightBack += spotLightColor[ i ] * spotLightWeightingBack * lDistance * spotEffect;

            #endif

        }

    }
    #endif

    #if MAX_HEMI_LIGHTS > 0

    for( int i = 0; i < MAX_HEMI_LIGHTS; i ++ ) {

        vec4 lDirection = viewMatrix * vec4( hemisphereLightDirection[ i ], 0.0 );
        vec3 lVector = normalize( lDirection.xyz );

        float dotProduct = dot( transformedNormal, lVector );

        float hemiDiffuseWeight = 0.5 * dotProduct + 0.5;
        float hemiDiffuseWeightBack = -0.5 * dotProduct + 0.5;

        vLightFront += mix( hemisphereLightGroundColor[ i ], hemisphereLightSkyColor[ i ], hemiDiffuseWeight );

        #ifdef DOUBLE_SIDED

            vLightBack += mix( hemisphereLightGroundColor[ i ], hemisphereLightSkyColor[ i ], hemiDiffuseWeightBack );

        #endif

    }
    #endif

    vLightFront = vLightFront * diffuse + ambient * ambientLightColor + emissive;

    #ifdef DOUBLE_SIDED

    vLightBack = vLightBack * diffuse + ambient * ambientLightColor + emissive;

    #endif
}
