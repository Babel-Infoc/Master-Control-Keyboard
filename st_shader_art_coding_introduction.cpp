// Shader Art Coding Introduction
// https://www.shadertoy.com/view/mtyGWy

#include "st_shader_art_coding_introduction.h"
#include "egl_missing.h"


namespace egl{

vec3f palette( float t ) {
    vec3f a = vec3f(0.5, 0.5, 0.5);
    vec3f b = vec3f(0.5, 0.5, 0.5);
    vec3f c = vec3f(1.0, 1.0, 1.0);
    vec3f d = vec3f(0.263,0.416,0.557);

    vec3f f = 6.28318*(c*t+d);

    return a + b * vec3f(cos(f.x), cos(f.y), cos(f.z));
}

}

using namespace egl;
vec4f shader_art_coding_introduction(vec2f fragCoord, vec2f iResolution, float iTime) {
    vec2f uv = (fragCoord * 2.0 - iResolution) / iResolution.y;
    vec2f uv0 = uv;
    vec3f finalColor = vec3f(0.0);
    
    for (int i = 0; i < 4; i++) {
        uv = fract(uv * 1.5) - 0.5;
        
        float d = norm(uv) * exp(-norm(uv0));

        vec3f col = palette(norm(uv0) + i*.4 + iTime*.4);

        d = sin(d*8. + iTime) * 0.125;
        d = abs(d);

        d = pow(d * 100 , -1.2);

        finalColor += col * d;
    }

    vec4f fragColor = vec4f(finalColor, 1.0);
    return fragColor;
}
