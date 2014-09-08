attribute vec3  inVertex;
attribute vec3  inNormal;
attribute vec2  inTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat4  ShadowProj;
uniform vec3  LightDirModel;
uniform vec3  EyePosModel;

varying vec2   TexCoord;
varying vec3   ShadowCoord;
varying vec2   ReflectCoord;
varying float  LightIntensity;

void main()
{
	// Transform position
	gl_Position = projectionMatrix*(viewMatrix*(modelMatrix*vec4(inVertex,1.0)));
		
	// Simple diffuse lighting
	LightIntensity = max(dot(inNormal, LightDirModel), 0.0);
	
	// Calculate eye direction in model space
	vec3 eyeDir = normalize(inVertex - EyePosModel);
	
	mat3 ModelWorld = mat3(modelMatrix);
	// reflect eye direction over normal and transform to world space
	ReflectCoord = vec2(ModelWorld * reflect(eyeDir, inNormal)) * 0.5 + 0.5;
	
	ShadowCoord = (ShadowProj * vec4(inVertex, 1.0)).xyw;
	ShadowCoord.xy += ShadowCoord.z;
	ShadowCoord.z *= 2.0;
	
	// Pass through texcoords
	TexCoord = inTexCoord;
}