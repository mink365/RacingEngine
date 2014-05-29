//[VERTEX SHADER]
//#version 330
 
attribute vec3 InVertex;
attribute vec4 InColor;
 
 
varying vec4 Color;
 
uniform mat4 ProjectionModelviewMatrix;
 
 
void main()
{
        gl_Position = ProjectionModelviewMatrix * vec4(InVertex, 1.0);
        Color = InColor;
}
