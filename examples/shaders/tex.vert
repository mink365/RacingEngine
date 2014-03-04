uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

attribute vec3 vertex;
attribute vec2 texCoord;

varying vec2 tc;
void main()
{
    tc = texCoord;

    gl_Position = projection*(view*(model*vec4(vertex,1.0)));
}
