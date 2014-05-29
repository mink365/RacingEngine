//[VERTEX SHADER]
//#version 330
 
attribute vec3 InVertex;
attribute vec3 InNormal;
 
 
varying vec3 LightVector0;
varying vec3 EyeNormal;
 
uniform mat4 ProjectionModelviewMatrix;
 
 
void main()
{
        gl_Position = ProjectionModelviewMatrix * vec4(InVertex, 1.0);
 
        LightVector0 = vec3(1.0, 1.0, 1.0);
        EyeNormal = InNormal;
}
