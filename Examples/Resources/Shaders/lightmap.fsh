uniform sampler2D  sBasetex;
uniform sampler2D  sReflect;
uniform sampler2D  sShadow;

varying vec2   TexCoord;
varying vec3   ShadowCoord;
varying vec2   ReflectCoord;
varying float  LightIntensity;

const float  cReflect = 0.2;

void main()
{
	vec3 baseColor = texture2D(sBasetex, TexCoord).rgb;
	baseColor *= 0.2 + 0.8 * texture2DProj(sShadow, ShadowCoord).r * LightIntensity;
	
	vec3 reflectColor = texture2D(sReflect, ReflectCoord).rgb;

	gl_FragColor = vec4(baseColor +  reflectColor * cReflect, 1.0);
}