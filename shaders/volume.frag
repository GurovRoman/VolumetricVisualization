out vec4 color;
noperspective in vec3 fragPos;

uniform mat4 VP_inv;

uniform sampler2D depth;


uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform float sunIntensity;




vec4 marchRay(vec3 begin, vec3 end) {
    vec3 final_color;
    float acc_transparency = 1.;

    vec3 sunDirection = normalize(sunDirection);

    vec3 step_delta = (end - begin) / vol.samples;
    float step_length = length(step_delta);

    mat4 model = inverse(vol.model_inv);
    vec3 world_begin = (model * vec4(begin - 0.5, 1)).xyz;
    vec3 world_end = (model * vec4(end - 0.5, 1)).xyz;
    vec3 world_step_delta = (world_end - world_begin) / vol.samples;

    for (int i = 0; i < vol.samples; ++i) {
        vec3 pos = begin + step_delta * i;
        float density = sample_density(pos);

        float opacity = density * step_length;

        vec3 world_pos = world_begin + world_step_delta * i;
        float volume_shadow = marchTransparency(pos, pos - sunDirection * 1, vol.shadow_samples);
        float opaque_shadow = calculateShadowFromMap(world_pos, sunShadowmap, 0);
        float shadowTransparency = clamp(volume_shadow - opaque_shadow, 0, 1);

        vec3 intensity = vec3(0.1) + shadowTransparency;
        vec3 color = intensity * sunColor * sunIntensity;

        final_color += color * min(1, opacity) * acc_transparency;
        acc_transparency *= exp(-opacity);
    }
    return vec4(final_color, 1 - acc_transparency);
}



void main(){
    vec4 origin4 = vec4(fragPos.xy, -1, 1);
    float current_buffer_depth = texture(depth, (fragPos.xy + 1) / 2).r * 2 - 1;
    vec4 end4 = vec4(fragPos.xy, current_buffer_depth, 1);           // either closest opaque or far plane

    origin4 = vol.model_inv * VP_inv * origin4;
    end4 = vol.model_inv * VP_inv * end4;

    vec3 origin = origin4.xyz / origin4.w + 0.5;
    vec3 end = end4.xyz / end4.w + 0.5;

    vec3 dir = normalize(end - origin);

    float in_dist, out_dist;
    BBoxIntersect(vec3(0), vec3(1), origin, dir, in_dist, out_dist);
    float end_dist = length(end - origin);

    if (end_dist < in_dist) {   // far plane or opaque covers the volume
        discard;
    }

    end = origin + dir * min(end_dist, out_dist);
    origin = origin + dir * max(0., in_dist);

    color = marchRay(origin, end);
}