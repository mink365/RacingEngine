uniform sampler2D  sBaseTex;
uniform sampler2D  sNormalMap;
		
varying vec3  LightVec;
varying vec2  TexCoord;

void main()
{
	// read the per-pixel normal from the normal map and expand to [-1, 1]
	vec3 normal = texture2D(sNormalMap, TexCoord).rgb * 2.0 - 1.0;
	
	// linear interpolations of normals may cause shortened normals and thus
	// visible artifacts on low-poly models.
	// We omit the normalization here for performance reasons
	
	// calculate diffuse lighting as the cosine of the angle between light
	// direction and surface normal (both in surface local/tangent space)
	// We don't have to clamp to 0 here because the framebuffer write will be clamped
	float lightIntensity = dot(LightVec, normal);

	// read base texture and modulate with light intensity
	vec3 texColor = texture2D(sBaseTex, TexCoord).rgb;	
	gl_FragColor = vec4(texColor * lightIntensity, 1.0);
}
