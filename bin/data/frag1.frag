#version 150
uniform mat4 invMatrix;
uniform vec3 lightPosition;
uniform sampler2DRect d_texture;
in vec3 vPosition;
in vec3 vNormal;
in vec4 vColor;
in vec4 vTexCoord;
in vec4 vDepth;
out vec4 outColor;

float restDepth(vec4 RGBA){
    const float rMask = 1.0;
    const float gMask = 1.0 / 255.0;
    const float bMask = 1.0 / (255.0 * 255.0);
    const float aMask = 1.0 / (255.0 * 255.0 * 255.0);
    float depth = dot(RGBA, vec4(rMask, gMask, bMask, aMask));
    return depth;
}

void main(){
    vec3  light     = lightPosition - vPosition;
    vec3  invLight  = normalize(invMatrix * vec4(light, 0.0)).xyz;
    float diffuse   = clamp(dot(vNormal, invLight), 0.2, 1.0);
    float shadow    = restDepth(textureProj(d_texture, vTexCoord));
    vec4 depthColor = vec4(1.0);
    if(vDepth.w > 0.0){
        vec4 lightCoord = vDepth / vDepth.w;
        if(lightCoord.z > shadow){
            depthColor  = vec4(0.5, 0.5, 0.5, 1.0);
        }
    }
//    outColor = vec4(vec3(length(vPosition)), 1.0);
    outColor = vColor * vec4(vec3(diffuse), 1.0) * depthColor;
//    outColor = textureProj(d_texture, vTexCoord);
//    outColor = vec4(vec3(shadow), 1.0);
    
}