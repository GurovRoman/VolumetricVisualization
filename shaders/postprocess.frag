#version 330 core

uniform sampler2D inColorTex;

in vec3 fragPos;
layout(location = 0) out vec4 color;

uniform float whitePoint = 1.0;


vec3 toneMap(vec3 inColor) {
    return inColor * (1.0f + (inColor / vec3(whitePoint * whitePoint))) / (1 + inColor);
}


void main() {
    vec2 texCoord = (fragPos.xy + 1) / 2;

    vec3 inColor = texture(inColorTex, texCoord).rgb;

    color.rgb = toneMap(inColor);
}
