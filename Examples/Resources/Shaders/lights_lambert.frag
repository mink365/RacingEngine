
uniform sampler2D textureSampler;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;

// -----------------------------------------------
uniform float opacity;

varying vec3 vLightFront;

#ifdef DOUBLE_SIDED

    varying vec3 vLightBack;

#endif


void main() {

    // ----------------------- light ------------------

    gl_FragColor = vec4( vec3( 1.0 ), opacity );

    #ifdef DOUBLE_SIDED

        //float isFront = float( gl_FrontFacing );
        //gl_FragColor.xyz *= isFront * vLightFront + ( 1.0 - isFront ) * vLightBack;

        if ( gl_FrontFacing )
            gl_FragColor.xyz *= vLightFront;
        else
            gl_FragColor.xyz *= vLightBack;

    #else

        gl_FragColor.xyz *= vLightFront;

    #endif

    // ------------------------------ normal ---------------

    vec4 textureColor = texture2D(textureSampler, vTexCoord);

    gl_FragColor *= vFragmentColor * textureColor;

}
