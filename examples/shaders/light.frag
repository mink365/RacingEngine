// Phong lighting
// note - not optimized!
varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vPos;
uniform sampler2D sTexture;
const vec3 lightPos = vec3(5.0, 5.0, 5.0);
const float shininess = 60.0;
const vec3 diffuseColor = vec3(1.0, 0.0, 0.0);
const vec3 specularColor = vec3(1.0, 1.0, 1.0);

void main() {
    vec3 n = normalize(vNormal);
    vec3 v = normalize(-vPos);
    vec3 l = normalize(lightPos - vPos);
    vec3 h = normalize(v + l);
    
    float diff = max(0.0, dot(n, l));
    float spec = pow(dot(n, h), shininess);
    float edge = 1.0 - abs(dot(v, n)); edge = edge*edge;

    vec3 colorMap = texture2D(sTexture, vTexCoord).xyz;

    gl_FragColor = texture2D(sTexture, vTexCoord);
    //gl_FragColor = vec4(vNormal.xyz*0.5+0.5, 1.0);
    //gl_FragColor = vec4(diff);
    //gl_FragColor = vec4(spec);
    //gl_FragColor = vec4(edge); 
    //gl_FragColor = vec4(diffuseColor*diff +  specularColor*spec + vec3(edge), 1.0);
//    gl_FragColor = vec4(colorMap*diff +  specularColor*spec + vec3(edge*0.5), 1.0);
}
