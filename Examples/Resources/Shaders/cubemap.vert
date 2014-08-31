 
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
 
attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aColor;

varying vec3 vTexCoord;
varying vec4 vFragmentColor;
 
void main(void)
{
    // Pass on the texture coordinates
    vTexCoord.xyz = normalize (aPosition.xyz);
 
    // Don't forget to transform the geometry!
    gl_Position = projectionMatrix*(viewMatrix*(modelMatrix*vec4(aPosition,1.0)));
}
