#version 150
in vec4 position;
uniform mat4 lgtMatrix;
//uniform mat4 modelViewProjectionMatrix;
out vec4 vPosition;

void main(void){
    vPosition = lgtMatrix * position;
    gl_Position = vPosition;
}
