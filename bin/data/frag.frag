#version 150
#define LIGHT_NUM 1

uniform mat4 invMatrix;
uniform vec3 lightPosition[LIGHT_NUM];
uniform sampler2DRect d_texture[LIGHT_NUM];
uniform bool active_light[LIGHT_NUM];
uniform vec4 ambientColor;
uniform float clipD;

uniform float tolerate;

in vec4 vPosition;
in vec3 vNormal;
in vec4 vColor;
in vec4 vTexCoord[LIGHT_NUM];
in vec4 vDepth[LIGHT_NUM];
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
    vec4 final = vec4(0.);
    vec4 test = vec4(0.5, 1.0, 1.0, 1.0);
    
    for (int i = 0; i < LIGHT_NUM; i++) {
        if (active_light[i]) {
            float lightCoord = 0.0;
            vec3  light     = lightPosition[i] - vPosition.xyz;
            vec3  invLight  = normalize(invMatrix * vec4(light, 0.0)).xyz;
            float diffuse   = clamp(dot(vNormal, invLight), 0.2, 1.0);
//            float shadow    = restDepth(textureProj(d_texture[i], vTexCoord[i]));
            float shadow    = textureProj(d_texture[i], vTexCoord[i]).r;
//            float shadow = textureProj(d_texture[i], vTexCoord[i]).x;
            vec4 depthColor = vec4(1.0);
            if(vDepth[i].w > 0.0){
//                vec4 lightCoord = vDepth[i] / vDepth[i].w; // 正規化デバイス座標系に変換
                lightCoord = (vDepth[i].z / clipD);
                if(lightCoord > shadow){
                    depthColor  = vec4(0.5, 0.5, 0.5, 1.0);
                    test = vec4(1.0, 1.0, 0.5, 1.0);
                }
            }
            
            final = vec4(vec3(diffuse), 1.0) * depthColor;
//            final = vec4(vec3(shadow/tolerate), 1.0);
//            final = depthColor;
        }
    }
//    outColor = vec4(vec3(length(vPosition)), 1.0);
    outColor = final;
//    outColor = textureProj(d_texture[0], vTexCoord[0]);
//    outColor = vec4(vec3(restDepth(textureProj(d_texture[0], vTexCoord[0]))),1.);
//    outColor = test;
//    outColor = vec4(vec3((vDepth[0] / vDepth[0].w).z), 1.0);
}