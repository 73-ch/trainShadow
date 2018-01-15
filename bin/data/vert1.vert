#version 150

in vec4 position;
in vec4 normal;
in vec4 color;
in vec2 texcoord;
uniform mat4 mMatrix;
uniform mat4 mvpMatrix;
uniform mat4 tMatrix;
uniform mat4 lgtMatrix;
out vec3 vPosition;
out vec3 vNormal;
out vec4 vColor;
out vec4 vTexCoord;
out vec4 vDepth;


void main() {
    vPosition = (mMatrix * position).xyz;
    vNormal = normal.xyz;
    vColor = color;
    vTexCoord = tMatrix * vec4(vPosition, 1.0);
    vDepth = lgtMatrix * position;
    gl_Position = mvpMatrix * position;
}                   