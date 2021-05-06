#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

out attributes {
    vec3 pos;
    vec3 normal;
    vec2 uv;
} vs_out;


uniform mat4 VP;
uniform mat4 model;
uniform mat3 normalMatrix;


void main(){
    gl_Position = VP * model * vec4(vertexPos, 1);
    vs_out.pos = (model * vec4(vertexPos, 1)).xyz;
    vs_out.normal = normalMatrix * vertexNormal;
    vs_out.uv = vertexUV;
}