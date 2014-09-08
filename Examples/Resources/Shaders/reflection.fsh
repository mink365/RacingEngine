uniform sampler2D s2DMap;
uniform samplerCube sCubeMap;

uniform bool bCubeReflection;

varying vec3  reflectDir;

void main()
{
	// select whether to use cube map reflection or 2d reflection	
	if(bCubeReflection)
	{
		gl_FragColor = textureCube(sCubeMap, reflectDir);
	}
	else 
	{
		gl_FragColor = texture2D(s2DMap, reflectDir.xy * 0.5 + 0.5);
	}
}