// Oscilloscope 
// https://www.shadertoy.com/view/4d3fD4

#include "st_oscilloscope.h"
#include "egl_missing.h"

namespace egl{

#define M_PI 3.1415926535897932384626433832795
#define border(B,X) smoothstep(B, (B)+1e-4, (X))
#define saturate(X) clamp((float)(X),(float)0.0,(float)1.0)

#define drawPointX 0.75

vec4f screenColor(float x) {
	return vec4f(x*0.75,x,0.0,1.0);
}

}

using namespace egl;
vec4f oscilloscope(vec2f fragCoord, vec2f iResolution, float iTime)
{
    vec4f fragColor;
	// Normalized pixel coordinates (from 0 to 1)
	vec2f uv = fragCoord/iResolution;
	
	float tx = uv.x + iTime * 0.3;
	float dy = uv.y + 0.2*sin(20.0*tx);
	float sinLine = saturate(border(0.9999, sin(M_PI * dy)) - (0.75 - uv.x));
	if(uv.x > drawPointX) { sinLine = 0.0; }
	
	float hLines = border(0.99, sin(M_PI * uv.y * 20.0 + 0.1));
	float vLines = border(0.99, sin(M_PI * uv.x * 32.0 + 0.1));
	float grid = 0.5 * saturate(hLines + vLines);
	
	float drawDot = smoothstep(.88, .89, .9 - length(vec2f(uv.x,dy)-vec2f(drawPointX,0.5)));
	
	// Output to screen
	fragColor = screenColor(saturate(sinLine + grid + drawDot));
    return fragColor;
}
