attribute vec3  aPosition;
attribute vec3  aNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3  eyePosModel;

varying vec3  reflectDir;

void main()
{
	// Transform position
	gl_Position = projectionMatrix*(viewMatrix*(modelMatrix*vec4(aPosition,1.0)));
	
	// Calculate eye direction in model space
	vec3 eyeDir = normalize(aPosition - eyePosModel);
	
	// reflect eye direction over normal and transform to world space 
	reflectDir = mat3(modelMatrix) * reflect(eyeDir, aNormal);
}