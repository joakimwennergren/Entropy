#version 450

// Common
#define OBJ_TYPE_ROUNDED_RECTANGLE_FRAME 2
layout(set = 1, binding = 2) uniform sampler2D Sampler2D;

layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV0;
layout (location = 3) in vec4 inColor0;
layout (location = 4) in vec4 inColor1;
layout (location = 5) flat in int objectType;
layout (location = 6) in vec2 inResolution;
layout (location = 7) flat in uint inHasTexture;

layout(location = 0) out vec4 outColor;

//layout(set = 1, binding = 1) uniform sampler texSampler;
//layout(set = 1, binding = 2) uniform texture2D _texture;s
// 2D
//---------------------------------------------------------
// draw rectangle frame with rounded edges
//---------------------------------------------------------
float roundedFrame (vec2 pos, vec2 uv, vec2 size, float radius, float thickness)
{
    float d = length(max(abs(uv - pos), size) - size) - radius;
    return smoothstep(0.55, 0.45, abs(d / thickness) * 5.0);
}

// s = side length
// r = corner radius
float sdRoundSquare( in vec2 p, in float s, in float r ) 
{
    vec2 q = abs(p)-s+r;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r;
}void RoundedRectangleFrame()
{
    // pixel and moust coordinates
	vec2 p = (2.0*gl_FragCoord.xy-inResolution.xy)/inResolution.y; // inUV0;

    // animate the square
	float si = 0.5; // float s = 0.5 - (radius + (thickness / 10));
    float ra = 0.1;

    // draw the square 
	float d = sdRoundSquare( p, si, ra );

    // apply colors to it
	vec4 col = mix(vec4(1.0, 0.0, 0.0, 1.0), inColor0, 1.0-smoothstep(0.0,0.01,abs(d)) );

	outColor = col;
}

void main()
{
    switch (objectType) {

        // Rounded rectangle frame
        case OBJ_TYPE_ROUNDED_RECTANGLE_FRAME:
            RoundedRectangleFrame();
        break;

        default:
            outColor = vec4(1.0, 0.0, 1.0, 1.0);
        break;

    }
}
