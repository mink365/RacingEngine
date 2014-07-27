uniform sampler2D textureSampler;
uniform sampler2D textureSampler2;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;

void main()
{
    vec4 textureColor = texture2D(textureSampler, vTexCoord);

    vec4 textureColor2 = texture2D(textureSampler2, vTexCoord);

    gl_FragColor = vFragmentColor * textureColor * textureColor2;
}
