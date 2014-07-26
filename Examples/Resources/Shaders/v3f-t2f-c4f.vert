
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aColor;
void main()
{
    gl_TexCoord[0].xy = aTexCoord.xy;
    gl_FrontColor     = aColor;
    gl_Position       = projectionMatrix*(viewMatrix*(modelMatrix*vec4(aPosition,1.0)));
}
