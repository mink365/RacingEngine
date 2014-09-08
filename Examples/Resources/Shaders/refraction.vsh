attribute vec3  aPosition;
attribute vec3  aNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3  lightDirModel;
uniform vec3  eyePosModel;
uniform bool  bSpecular;
uniform	bool  bRotate;

varying float  specularIntensity;
varying vec2   refractCoord;

const float  cShininess = 3.0;
const float  cRIR = 1.015;

void main()
{
	mat4 MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;

	// Transform position
	gl_Position = MVPMatrix * vec4(aPosition, 1.0);
	
	// Eye direction in model space
	vec3 eyeDirModel = normalize(aPosition - eyePosModel);	
	
	// GLSL offers a nice built-in refaction function
	// Calculate refraction direction in model space
	vec3 refractDir = refract(eyeDirModel, aNormal, cRIR);
	
	// Project refraction
	refractDir = (MVPMatrix * vec4(refractDir, 0.0)).xyw;

	// Map refraction direction to 2d coordinates
	refractCoord = 0.5 * (refractDir.xy / refractDir.z) + 0.5;

	if(bRotate) // If the screen is rotated then rotate the uvs
	{
		refractCoord.xy = refractCoord.yx;
		refractCoord.y = -refractCoord.y;
	}
		
	// Specular lighting
	// We ignore that N dot L could be negative (light coming 
	// from behind the surface)
	specularIntensity = 0.0;

	if (bSpecular)
	{
		vec3 halfVector = normalize(lightDirModel + eyeDirModel);
		float NdotH = max(dot(aNormal, halfVector), 0.0);		
		specularIntensity = pow(NdotH, cShininess);
	}
}