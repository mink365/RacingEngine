 
uniform samplerCube textureUnit0;
 
varying vec3 vTexCoord;
varying vec4 vFragmentColor;
 
void main(void)
{
    gl_FragColor = textureCube (textureUnit0, vTexCoord);
}
