uniform sampler2D textureSampler;

varying vec2 tc;

void main()
{
    vec4 textureColor = texture2D(textureSampler, tc);

    gl_FragColor = textureColor;
}
