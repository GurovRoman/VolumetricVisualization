uniform sampler2D gAlbedo;
uniform sampler2D gPos;
uniform sampler2D gNormal;

uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform float sunIntensity;

uniform vec3 cameraPosition;

in vec3 fragPos;
layout(location = 0) out vec4 color;


float computeVolumeShadow(vec3 pos, vec3 dir) {
    dir = (vol.model_inv * vec4(pos + dir, 1)).xyz + 0.5;
    pos = (vol.model_inv * vec4(pos, 1)).xyz + 0.5;
    dir -= pos;

    float in_dist, out_dist;
    if (!BBoxIntersect(vec3(0), vec3(1), pos, dir, in_dist, out_dist)) {
        return 1;
    }

    vec3 end = pos + dir * out_dist;
    vec3 origin = pos + dir * max(0., in_dist);

    return marchTransparency(origin, end, vol.scene_shadow_samples);
}


void main() {
    vec2 texCoord = (fragPos.xy + 1) / 2;

    vec3 albedo = texture(gAlbedo, texCoord).rgb;
    float specular_intensity = texture(gAlbedo, texCoord).a;
    vec3 pos = texture(gPos, texCoord).xyz;
    vec3 normal = texture(gNormal, texCoord).xyz;

    if (length(normal) == 0) {
        discard;
    }

    normal = normalize(normal);

    vec3 sunDirection = normalize(sunDirection);

    vec3 hdrColor = sunColor * sunIntensity;

    vec3 ambient = albedo * hdrColor * 0.1;

    float shadow = clamp(computeVolumeShadow(pos, -sunDirection) - calculateShadowFromMap(pos, sunShadowmap, 0), 0, 1);

    vec3 diffuse = albedo * hdrColor * max(0, dot(-sunDirection, normal));
    vec3 specular = specular_intensity * hdrColor * pow(max(0, dot(reflect(sunDirection, normal), normalize(cameraPosition - pos))), 4);

    // if the light hits face from behind, specular should be forced to 0
    if (dot(-sunDirection, normal) <= 0) {
        specular = vec3(0);
    }

    color = vec4(ambient + shadow * (diffuse + specular), 1);
    //color = vec4(texture(sunShadowmap.depthTex, texCoord).rgb, 1);
}
