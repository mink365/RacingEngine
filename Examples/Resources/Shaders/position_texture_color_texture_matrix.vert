uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 textureM;

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aColor;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;

void main()
{
    vec4 texCoord = textureM * vec4(aTexCoord, 1.0, 1.0);
	vTexCoord = texCoord.xy;
    vFragmentColor = aColor;

    gl_Position = projection*(view*(model*vec4(aPosition,1.0)));
}
