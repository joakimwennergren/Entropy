#version 450

//push constants block
layout(push_constant) uniform constants {
    uint instanceIndex;
} PushConstants;

struct InstanceData{
    mat4 model;
    vec4 bgColor;
    vec4 borderColor;
    vec2 dimension;
    int type;
};

//all object matrices
layout(std140, set = 0, binding = 0) readonly buffer InstanceBuffer{
    InstanceData objects[];
} instanceBuffer;

// Common
#define OBJ_TYPE_ROUNDED_RECTANGLE_FRAME 2
layout(set = 1, binding = 2) uniform sampler2D Sampler2D;

layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;
layout (location = 3) in vec4 Color;

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
float sdRoundSquare(in vec2 p, in float s, in float r)
{
    vec2 q = abs(p)-s+r;
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r;
}

float RectSDF(vec2 p, vec2 b, float r)
{
    vec2 d = abs(p) - b + vec2(r);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
}

float roundedBoxSDF(vec2 CenterPosition, vec2 Size, vec4 Radius)
{
    Radius.xy = (CenterPosition.x > 0.0) ? Radius.xy : Radius.zw;
    Radius.x  = (CenterPosition.y > 0.0) ? Radius.x  : Radius.y;

    vec2 q = abs(CenterPosition)-Size+Radius.x;
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - Radius.x;
}void RoundedRectangleFrame()
{
    /*
    // pixel and moust coordinates
    vec2 p = (2.0*gl_FragCoord.xy-inResolution.xy)/inResolution.y;// inUV0;

    // animate the square
    float si = 0.5;// float s = 0.5 - (radius + (thickness / 10));
    float ra = 0.1;

    // draw the square
    float d = sdRoundSquare(p, si, ra);

    // apply colors to it
    vec4 col = mix(vec4(1.0, 0.0, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 1.0), 1.0-smoothstep(0.0, 0.01, abs(d)));

    outColor = col;
    */

    /*
    const vec4 borderColor = vec4(0.0, 1.0, 0.5, 1.0);
    const vec4 fillColor = vec4(1.0, 0.0, 0.0, 0.0);
    const float borderThickness = 5.0;
    const float radius = 12.0;
    const vec4  u_cornerRadiuses = vec4(10.0, 20.0, 40.0, 60.0);
    const vec2 rectSize = vec2(300.0, 300.0);

    vec2 pos = rectSize * inUV0;

    float fDist = roundedBoxSDF(pos-rectSize/2.0, rectSize/2.0 - borderThickness/2.0-1.0, u_cornerRadiuses);
    float fBlendAmount = smoothstep(-1.0, 1.0, abs(fDist) - borderThickness / 2.0);

    vec4 v4FromColor = borderColor;
    vec4 v4ToColor = (fDist < 0.0) ? fillColor : vec4(0.0);
    outColor = mix(v4FromColor, v4ToColor, fBlendAmount);
    */

    // =========================================================================
    // Inputs (uniforms)

    vec2  u_rectSize   = instanceBuffer.objects[PushConstants.instanceIndex].dimension;// The pixel-space scale of the rectangle.
    vec2  u_rectCenter = (u_rectSize * UV);// The pixel-space rectangle center location
    vec2  u_offset = vec2(20, 20);

    float u_edgeSoftness   = 1.0;// How soft the edges should be (in pixels). Higher values could be used to simulate a drop shadow.
    vec4  u_cornerRadiuses = vec4(0.0, 40.0, 10.0, 0.0);// The radiuses of the corners(in pixels): [topRight, bottomRight, topLeft, bottomLeft]

    // Border
    float u_borderThickness = 2.0;// The border size (in pixels)
    float u_borderSoftness  = 2.0;// How soft the (internal) border should be (in pixels)

    // Shadow
    float u_shadowSoftness = 4.0;// The (half) shadow radius (in pixels)
    vec2  u_shadowOffset   = vec2(0.0, 0.0);// The pixel-space shadow offset from rectangle center

    // Colors
    vec4  u_colorBg     = vec4(0.0, 0.0, 0.0, 0.0);// The color of background
    vec4  u_colorRect   = instanceBuffer.objects[PushConstants.instanceIndex].bgColor;// The color of rectangle
    vec4  u_colorBorder = vec4(80.0 / 255.0, 96.0 / 255.0, 99.0 / 255.0, 1.0);// The color of (internal) border
    vec4  u_colorShadow = vec4(0.4, 0.4, 0.4, 1.0);// The color of shadow

    // =========================================================================

    vec2 halfSize = (u_rectSize / 2.0);// Rectangle extents (half of the size)

    // -------------------------------------------------------------------------

    // Calculate distance to edge.
    float distance = roundedBoxSDF(u_rectCenter-halfSize, halfSize - u_offset, u_cornerRadiuses);

    // Smooth the result (free antialiasing).
    float smoothedAlpha = 1.0-smoothstep(0.0, u_edgeSoftness, distance);

    // -------------------------------------------------------------------------
    // Border.

    float borderAlpha   = 1.0-smoothstep(u_borderThickness - u_borderSoftness, u_borderThickness, abs(distance));

    // -------------------------------------------------------------------------
    // Apply a drop shadow effect.

    float shadowDistance  = roundedBoxSDF(u_rectCenter-halfSize + u_shadowOffset, halfSize - u_offset, u_cornerRadiuses);
    float shadowAlpha      = 1.0-smoothstep(-u_shadowSoftness, u_shadowSoftness, shadowDistance);


    // -------------------------------------------------------------------------
    // Debug output

    // vec4 debug_sdf = vec4(distance, 0.0, 0.0, 1.0);

    // Notice, that instead simple 'alpha' here is used 'min(u_colorRect.a, alpha)' to enable transparency
    // vec4 debug_rect_color   = mix(u_colorBg, u_colorRect, min(u_colorRect.a, smoothedAlpha));

    // Notice, that instead simple 'alpha' here is used 'min(u_colorBorder.a, alpha)' to enable transparency
    // vec4 debug_border_color = mix(u_colorBg, u_colorBorder, min(u_colorBorder.a, min(borderAlpha, smoothedAlpha)) );

    // -------------------------------------------------------------------------
    // Apply colors layer-by-layer: background <- shadow <- rect <- border.

    // Blend background with shadow
    vec4 res_shadow_color = mix(u_colorBg, vec4(u_colorShadow.rgb, shadowAlpha), shadowAlpha);

    // Blend (background+shadow) with rect
    //   Note:
    //     - Used 'min(u_colorRect.a, smoothedAlpha)' instead of 'smoothedAlpha'
    //       to enable rectangle color transparency
    vec4 res_shadow_with_rect_color =
    mix(
    res_shadow_color,
    u_colorRect,
    min(u_colorRect.a, smoothedAlpha)
    );

    // Blend (background+shadow+rect) with border
    //   Note:
    //     - Used 'min(borderAlpha, smoothedAlpha)' instead of 'borderAlpha'
    //       to make border 'internal'
    //     - Used 'min(u_colorBorder.a, alpha)' instead of 'alpha' to enable
    //       border color transparency
    vec4 res_shadow_with_rect_with_border =
    mix(
    res_shadow_with_rect_color,
    u_colorBorder,
    min(u_colorBorder.a, min(borderAlpha, smoothedAlpha))
    );

    // -------------------------------------------------------------------------

    outColor = res_shadow_with_rect_with_border;
}

void main()
{
    switch (instanceBuffer.objects[PushConstants.instanceIndex].type) {

        case OBJ_TYPE_ROUNDED_RECTANGLE_FRAME:
        RoundedRectangleFrame();
        break;

        default :
        outColor = vec4(1.0, 0.0, 1.0, 1.0);
        break;

    }
}
