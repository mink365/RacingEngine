
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aColor;
void main()
{
    gl_TexCoord[0].xy = aTexCoord.xy;
    gl_FrontColor     = aColor;
    gl_Position       = projection*(view*(model*vec4(aPosition,1.0)));
}
