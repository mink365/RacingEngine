uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec4 aColor;

varying vec2 vTexCoord;
varying vec4 vFragmentColor;

varying vec4 vShadowCoord;
uniform mat4 shadowMatrix;

void main()
{
    vTexCoord = aTexCoord;
    vFragmentColor = aColor;

    gl_Position = projectionMatrix*(viewMatrix*(modelMatrix*vec4(aPosition,1.0)));

    vec4 worldPosition = modelMatrix * vec4( aPosition, 1.0 );

    vShadowCoord = shadowMatrix * worldPosition;
}