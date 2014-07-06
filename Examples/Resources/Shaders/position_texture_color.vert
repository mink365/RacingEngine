uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aColor;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;

void main()
{
    vTexCoord = aTexCoord;
    vFragmentColor = aColor;

    gl_Position = projection*(view*(model*vec4(aPosition,1.0)));
}
