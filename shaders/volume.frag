out vec4 color;
noperspective in vec3 fragPos;

uniform mat4 VP_inv;

uniform sampler2D depth;


uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform float sunIntensity;




vec4 marchRay(vec3 begin, vec3 end) {
    vec3 val;
    float acc_transparency = 1.;

    vec3 sunDirection = normalize(sunDirection);

    vec3 step_delta = (end - begin) / vol.samples;
    float step_length = length(step_delta);

    for (int i = 0; i < vol.samples; ++i) {
        vec3 pos = begin + step_delta * i;
        float density = sample_density(pos);

        float opacity = density * step_length;

        vec3 color_ = vec3(0.1) + marchTransparency(pos, pos - sunDirection * 1, vol.shadow_samples);
        color_ *= sunColor * sunIntensity;

        val += color_ * min(1, opacity) * acc_transparency;
        acc_transparency *= exp(-opacity);
    }
    return vec4(val, 1 - acc_transparency);
}



void main(){
    vec4 origin4 = vec4(fragPos.xy, -1, 1);
    float current_buffer_depth = texture(depth, (fragPos.xy + 1) / 2).r * 2 - 1;
    vec4 end4 = vec4(fragPos.xy, current_buffer_depth, 1);           // either closest opaque or far plane

    origin4 = vol.model_inv * VP_inv * origin4;
    end4 = vol.model_inv * VP_inv * end4;

    vec3 origin = origin4.xyz / origin4.w + 0.5;
    vec3 end = end4.xyz / end4.w + 0.5;
    float end_dist = length(end - origin);

    vec3 dir = normalize(end - origin);

    float in_dist, out_dist;
    BBoxIntersect(vec3(0), vec3(1), origin, dir, in_dist, out_dist);

    if (end_dist < in_dist) {   // far plane or opaque covers the volume
        discard;
    }

    end = origin + dir * min(end_dist, out_dist);
    origin = origin + dir * max(0., in_dist);

    color = marchRay(origin, end);
}