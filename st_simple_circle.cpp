// A simple circle
//https://www.shadertoy.com/view/XsjGDt

#include "st_simple_circle.h"
#include "egl_missing.h"

// Convert r, g, b to normalized vec3f
namespace egl{
vec3f rgb(float r, float g, float b) {
	return vec3f(r / 255.0, g / 255.0, b / 255.0);
}


// Draw a circle at vec2f `pos` with radius `rad` and color `color`.
vec4f circle(vec2f uv, vec2f pos, float rad, vec3f color) {
	float d = length(pos - uv) - rad;
	float t = clamp(d, (float)0.0, (float)1.0);
	return vec4f(color, 1.0 - t);
}

}


using namespace egl;
vec4f simple_circle(vec2f fragCoord, vec2f iResolution, float iTime) {
	vec4f fragColor;
	vec2f uv = fragCoord;
	vec2f center = iResolution * 0.5;
	float radius = 0.25 * iResolution.y;

    // Background layer
	vec4f layer1 = vec4f(rgb(210.0, 222.0, 228.0), 1.0);
	
	// Circle
	vec3f red = rgb(225.0, 95.0, 60.0);
	vec4f layer2 = circle(uv, center, radius, red);
	
	// Blend the two
	fragColor = mix(layer1, layer2, layer2.x);
	return fragColor;
}
