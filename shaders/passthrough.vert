#version 330 core

layout(location = 0) in vec3 vertexPos;


out vec3 fragPos;


void main(){
    gl_Position = vec4(vertexPos, 1);
    fragPos = gl_Position.xyz;
}