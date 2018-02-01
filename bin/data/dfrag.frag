#version 150
in vec4 vPosition;
in vec4 vColor;
out vec4 outColor;

vec4 convRGBA(float depth){
    float r = depth;
    float g = fract(r * 255.0);
    float b = fract(g * 255.0);
    float a = fract(b * 255.0);
    float coef = 1.0 / 255.0;
    r -= g * coef;
    g -= b * coef;
    b -= a * coef;
    return vec4(r, g, b, a);
}

void main(void){
//    outColor = convRGBA(gl_FragCoord.z);
    outColor = vec4(gl_FragCoord.z);
}