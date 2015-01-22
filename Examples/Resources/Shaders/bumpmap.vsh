attribute vec3  inVertex;
attribute vec3  inNormal;
attribute vec2  inTexCoord;
attribute vec3  inTangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3  LightPosModel;	// Light position (point light) in model space

varying vec3  LightVec;
varying vec2  TexCoord;

void main()
{
	// Transform position
	gl_Position = projectionMatrix*(viewMatrix*(modelMatrix*vec4(inVertex,1.0)));
	
	// Calculate light direction from light position in model space
	// You can skip this step for directional lights
	vec3 lightDirection = normalize(LightPosModel - inVertex);
	
	// transform light direction from model space to tangent space
	vec3 bitangent = cross(inNormal, inTangent);
	mat3 tangentSpaceXform = mat3(inTangent, bitangent, inNormal);
	LightVec = lightDirection * tangentSpaceXform;
	
	TexCoord = inTexCoord;
}
