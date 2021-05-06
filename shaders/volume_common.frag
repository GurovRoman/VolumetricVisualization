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