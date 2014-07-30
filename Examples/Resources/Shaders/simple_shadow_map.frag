uniform sampler2D textureSampler;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;

uniform sampler2D shadowMap;

uniform float shadowDarkness;
uniform float shadowBias;

varying vec4 vShadowCoord;

float unpackDepth( const in vec4 rgba_depth ) {
    const vec4 bit_shift = vec4( 1.0 / ( 256.0 * 256.0 * 256.0 ), 1.0 / ( 256.0 * 256.0 ), 1.0 / 256.0, 1.0 );
    float depth = dot( rgba_depth, bit_shift );
    return depth;
}

void main()
{
    vec4 textureColor = texture2D(textureSampler, vTexCoord);

    gl_FragColor = vFragmentColor * textureColor;


    // shadow calc

    vec3 shadowColor = vec3( 1.0 );

    vec3 shadowCoord = vShadowCoord.xyz / vShadowCoord.w;

    vec4 rgbaDepth = texture2D( shadowMap, shadowCoord.xy );
    float fDepth = unpackDepth( rgbaDepth );
    // float fDepth = rgbaDepth.x;

    if (fDepth < shadowCoord.z) {
    // if ( fDepth < 0.990000001 ) {
    // if ( shadowCoord.z < 0.998 ) {
        shadowColor = shadowColor * vec3( 1.0 - shadowDarkness );
    }

    gl_FragColor.xyz = gl_FragColor.xyz * shadowColor;
}