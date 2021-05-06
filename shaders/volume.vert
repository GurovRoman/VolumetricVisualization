#version 330 core

layout(location = 0) in vec3 vertexPos;

uniform mat4 MVP;

noperspective out vec3 fragPos;


void main(){
    gl_Position = MVP * vec4(vertexPos, 1);
    fragPos = gl_Position.xyz / gl_Position.w;
}