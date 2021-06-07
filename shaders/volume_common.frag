#version 330 core

// Volume Parameters
struct Volume {
    sampler3D density;
    mat4 model_inv;

    int samples;
    int shadow_samples;
    int scene_shadow_samples;
    float density_multiplier;
    bool use_worldspace_coordinates;
    bool procedural;
    float sample_point;
};


struct Shadowmap {
    sampler2DShadow depthTex;
    mat4 VP;
};


uniform Shadowmap sunShadowmap;
uniform Volume vol;


bool BBoxIntersect(const vec3 boxMin, const vec3 boxMax, const vec3 origin, const vec3 dir, out float begin, out float end) {
    vec3 invDir = 1. / dir;
    vec3 tbot = invDir * (boxMin - origin);
    vec3 ttop = invDir * (boxMax - origin);
    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);
    vec2 t = max(tmin.xx, tmin.yz);
    float t0 = max(t.x, t.y);
    t = min(tmax.xx, tmax.yz);
    float t1 = min(t.x, t.y);
    begin = t0;
    end = t1;
    return t1 > max(t0, 0.0);
}




float f(const vec3 x) {
    float cube_distance = length(max(abs(x - vec3(0.5)) - 0.40, 0.));
    cube_distance *= 10;
    cube_distance = 1 - clamp(cube_distance, 0, 1);
    return cube_distance;
}

float sample_density(const vec3 x) {
    if (vol.procedural)
        return f(x) * vol.density_multiplier;
    return texture(vol.density, x).r * vol.density_multiplier;
}

float marchTransparency(vec3 begin, const vec3 end, const int samples) {
    float acc_transparency = 0.;

    vec3 step_delta = (end - begin) / samples;

    begin += step_delta * vol.sample_point;

    float step_length = length(step_delta);
    for (int i = 0; i < samples; ++i) {
        vec3 pos = begin + step_delta * i;
        float opacity = sample_density(pos) * step_length;
        acc_transparency -= opacity;
    }
    return exp(acc_transparency);
}



float calculateShadowFromMap(const vec3 pos, const Shadowmap map, const float bias) {
    vec4 pos_proj = map.VP * vec4(pos, 1);
    pos_proj = pos_proj / 2 + 0.5;

    pos_proj.z -= bias * pos_proj.w;

    float shadow_opacity = 0;

    vec2 texelSize = 1.0 / textureSize(map.depthTex, 0);
    /*for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            shadow_opacity += textureProj(map.depthTex, vec4(pos_proj.xy + vec2(x, y) * texelSize * pos_proj.w, pos_proj.zw));
        }
    }
    shadow_opacity /= 9;*/

    vec2 offset = vec2(greaterThan(fract(pos_proj.xy / pos_proj.w * 0.5), vec2(0.25)));
    // mod
    offset.y += offset.x;
    // y ^= x in floating point
    if (offset.y > 1.1)
        offset.y = 0;
    shadow_opacity = (textureProj(map.depthTex, vec4(pos_proj.xy + vec2(-1.5,  0.5) * texelSize * pos_proj.w, pos_proj.zw))
                    + textureProj(map.depthTex, vec4(pos_proj.xy + vec2( 0.5,  0.5) * texelSize * pos_proj.w, pos_proj.zw))
                    + textureProj(map.depthTex, vec4(pos_proj.xy + vec2(-1.5, -1.5) * texelSize * pos_proj.w, pos_proj.zw))
                    + textureProj(map.depthTex, vec4(pos_proj.xy + vec2( 0.5, -1.5) * texelSize * pos_proj.w, pos_proj.zw))) * 0.25;

    float pos_depth = pos_proj.z / pos_proj.w;
    return pos_depth >= 1. ? 0 : shadow_opacity;
}