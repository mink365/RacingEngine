attribute highp vec3  inVertex;
attribute highp vec3  inNormal;
attribute highp vec2  inTexCoord;
attribute highp vec3  inTangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform highp vec3  LightPosModel;	// Light position (point light) in model space

varying lowp vec3  LightVec;
varying mediump vec2  TexCoord;

void main()
{
	// Transform position
	gl_Position = projectionMatrix*(viewMatrix*(modelMatrix*vec4(inVertex,1.0)));
	
	// Calculate light direction from light position in model space
	// You can skip this step for directional lights
	highp vec3 lightDirection = normalize(LightPosModel - inVertex);
	
	// transform light direction from model space to tangent space
	highp vec3 bitangent = cross(inNormal, inTangent);
	highp mat3 tangentSpaceXform = mat3(inTangent, bitangent, inNormal);
	LightVec = lightDirection * tangentSpaceXform;
	
	TexCoord = inTexCoord;
}
