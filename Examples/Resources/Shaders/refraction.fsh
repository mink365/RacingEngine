uniform sampler2D  sTexture;

varying float  specularIntensity;
varying vec2   refractCoord;

void main()
{
	vec3 refractColor = texture2D(sTexture, refractCoord).rgb;	
	gl_FragColor = vec4(refractColor + specularIntensity, 1.0);
}