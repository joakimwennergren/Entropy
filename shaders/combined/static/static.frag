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
}void RoundedRectangleFrame()
{
    vec4 frameColor = vec4(0.0, 0.0, 0.0, 1.0);
    float radius = 0.08;
    float thickness = 0.2;
    float s = 0.5 - (radius + (thickness / 10));
    vec2 size = vec2(s, s);
    vec2 pos = vec2(0.5, 0.5);

    // Normalize the pixel coordinates (this is "passTexCoords" in your case)
    vec2 uv = inUV0;//inUV0.xy/inResolution.xy;

    // (Note: iResolution.xy holds the x and y dimensions of the window in pixels)
    vec2 aspectRatio = vec2(inResolution.x/inResolution.y, 1.0);

    // In order to make sure visual distances are preserved, we multiply everything by aspectRatio
    //uv *= aspectRatio;
    //pos *= aspectRatio;
    //size *= aspectRatio;

    //--- rounded frame ---
    float intensity = roundedFrame(pos, uv, size, radius, thickness);
    vec4 col = mix(vec4(0.0, 0.0, 1.0, 0.0), frameColor, intensity);

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
