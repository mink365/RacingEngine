uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform vec2 resolution;

attribute vec3 aPosition;
attribute vec2 aTexCoord;
attribute vec3 aNormal;

varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vPos;

void main() {
    mat4 modelViewMatrix = view * model;
    mat4 mvpMatrix = projection * modelViewMatrix;

    gl_Position = mvpMatrix * vec4(aPosition, 1.0);
    vTexCoord = aTexCoord;

    // transform normal and vertex position to eye space

    mat3 normalMatrix = mat3(modelViewMatrix[0].xyz, modelViewMatrix[1].xyz, modelViewMatrix[2].xyz);

    vNormal = normalMatrix * aNormal;
    vPos = vec3(modelViewMatrix * vec4(aPosition, 1.0));
}
