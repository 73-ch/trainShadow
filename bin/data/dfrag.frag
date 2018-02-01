#version 150
in vec4 vPosition;
in vec4 vColor;
uniform float clipD;
out vec4 outColor;
void main(void){
    float depth = (vPosition.z / clipD);
    outColor = vec4(vec3(depth), 1.0);
}