#version 330 core

layout(location = 0) out vec4 albedo;
layout(location = 1) out vec3 pos;
layout(location = 2) out vec3 normal;

in attributes {
    vec3 pos;
    vec3 normal;
    vec2 uv;
} fs_in;

uniform vec4 solidColorSpec;

void main(){
    albedo = solidColorSpec;
    pos = fs_in.pos;
    normal = fs_in.normal;
}
