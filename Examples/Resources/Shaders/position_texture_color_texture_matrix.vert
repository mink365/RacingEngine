uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aColor;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;

void main()
{
    vec4 texCoord = textureMatrix * vec4(aTexCoord, 1.0, 1.0);
	vTexCoord = texCoord.xy;
    vFragmentColor = aColor;

    gl_Position = projectionMatrix*(viewMatrix*(modelMatrix*vec4(aPosition,1.0)));
}
