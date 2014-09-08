uniform sampler2D  sTexture;

varying lowp    float  specularIntensity;
varying mediump vec2   refractCoord;

void main()
{
	lowp vec3 refractColor = texture2D(sTexture, refractCoord).rgb;	
	gl_FragColor = vec4(refractColor + specularIntensity, 1.0);
}