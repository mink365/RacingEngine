//[FRAGMENT SHADER]
//#version 330
 
varying vec3 LightVector0;
varying vec3 EyeNormal;
 
void main()
{
        vec3 eyeNormal;
        vec3 lightVector;
        float dotProduct;
 
        eyeNormal = normalize(EyeNormal);
        lightVector = normalize(LightVector0);
 
        dotProduct = dot(eyeNormal, lightVector);
 
        gl_FragColor = vec4(dotProduct);
}
