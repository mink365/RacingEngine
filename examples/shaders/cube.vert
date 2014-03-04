uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 Color;
attribute vec3 vertex;
attribute vec4 color;
void main()
{
    gl_FrontColor = color*Color;
    gl_Position = projection*(view*(model*vec4(vertex,1.0)));
}
