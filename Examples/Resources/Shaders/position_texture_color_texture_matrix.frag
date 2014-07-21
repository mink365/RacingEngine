uniform sampler2D textureSampler;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;

void main()
{
    vec4 textureColor = texture2D(textureSampler, vTexCoord);

    gl_FragColor = vFragmentColor * textureColor;
}
