uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

attribute vec3 aPosition;
attribute vec4 aColor;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;

void main()
{
    vFragmentColor = aColor;

    gl_Position = projectionMatrix*(viewMatrix*(modelMatrix*vec4(aPosition,1.0)));
}