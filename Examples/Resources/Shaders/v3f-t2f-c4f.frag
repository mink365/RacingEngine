
uniform sampler2D textureSampler;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;

void main()
{
    float a = texture2D(textureSampler, vTexCoord).r;

    gl_FragColor = vec4(vFragmentColor.rgb, vFragmentColor.a*a);
}
