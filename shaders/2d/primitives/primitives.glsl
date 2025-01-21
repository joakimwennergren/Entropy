void RoundedRectangle()
{
    // https://andorsaga.wordpress.com/2018/06/26/sdfs-rendering-a-rectangle/

    // Dimensions
    vec2  u_rectSize   = instanceBuffer.objects[PushConstants.instanceIndex].dimension;// The pixel-space scale of the rectangle.
    vec2  u_rectCenter = (u_rectSize * UV);// The pixel-space rectangle center location
    vec2  u_offset = vec2(1, 1);

    // Corner radiuses
    float u_edgeSoftness   = 1.0;// How soft the edges should be (in pixels). Higher values could be used to simulate a drop shadow.
    vec4  u_cornerRadiuses = instanceBuffer.objects[PushConstants.instanceIndex].cornerRadiuses;// The radiuses of the corners(in pixels): [topRight, bottomRight, topLeft, bottomLeft]

    // Border
    float u_borderThickness = 1.0;// The border size (in pixels)
    float u_borderSoftness  = 2.0;// How soft the (internal) border should be (in pixels)

    // Shadow
    float u_shadowSoftness = 12.0;// The (half) shadow radius (in pixels)
    vec2  u_shadowOffset   = vec2(3.0, 3.0);// The pixel-space shadow offset from rectangle center

    // Colors
    vec4  u_colorBg     = vec4(0.0, 0.0, 0.0, 0.0);// The color of background
    vec4  u_colorRect   = instanceBuffer.objects[PushConstants.instanceIndex].bgColor;// The color of rectangle
    vec4  u_colorBorder = instanceBuffer.objects[PushConstants.instanceIndex].borderColor;// The color of (internal) border
    vec4  u_colorShadow = vec4(0.4, 0.4, 0.4, 1.0);// The color of shadow

    // =========================================================================

    vec2 halfSize = (u_rectSize / 2.0);// Rectangle extents (half of the size)

    // Calculate distance to edge.
    float distance = RoundedBoxSDF(u_rectCenter-halfSize, halfSize - u_offset, u_cornerRadiuses);

    // Smooth the result (free antialiasing).
    float smoothedAlpha = 1.0-smoothstep(0.0, u_edgeSoftness, distance);

    // Border 
    float borderAlpha   = 1.0-smoothstep(u_borderThickness - u_borderSoftness, u_borderThickness, abs(distance));

    // Apply a drop shadow effect.
    float shadowDistance  = RoundedBoxSDF(u_rectCenter-halfSize + u_shadowOffset, halfSize - u_offset, u_cornerRadiuses);
    float shadowAlpha      = 1.0-smoothstep(-u_shadowSoftness, u_shadowSoftness, shadowDistance);

    // -------------------------------------------------------------------------
    // Apply colors layer-by-layer: background <- shadow <- rect <- border.

    // Blend background with shadow
    vec4 res_shadow_color = mix(u_colorBg, vec4(0.0, 0.0, 0.0, 0.0), borderAlpha);

    // Blend (background+shadow) with rect
    //   Note:
    //     - Used 'min(u_colorRect.a, smoothedAlpha)' instead of 'smoothedAlpha'
    //       to enable rectangle color transparency
    vec4 res_shadow_with_rect_color = mix(
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
    vec4 res_shadow_with_rect_with_border = mix(
    res_shadow_with_rect_color,
    u_colorBorder,
    min(u_colorBorder.a, min(borderAlpha, smoothedAlpha))
    );

    // Finally output color
    outColor = res_shadow_with_rect_with_border;
}

void Circle()
{
    // Dimensions
    vec2  u_rectSize   = instanceBuffer.objects[PushConstants.instanceIndex].dimension;// The pixel-space scale of the rectangle.
    vec2  u_rectCenter = (u_rectSize * UV);// The pixel-space rectangle center location
    vec2  u_offset = vec2(20, 20);

    // Corner radiuses
    float u_edgeSoftness   = 1.0;// How soft the edges should be (in pixels). Higher values could be used to simulate a drop shadow.
    float u_cornerRadius   = 10;

    // Border
    float u_borderThickness = 0.0;// The border size (in pixels)
    float u_borderSoftness  = 2.0;// How soft the (internal) border should be (in pixels)

    // Shadow
    float u_shadowSoftness = 1.0;// The (half) shadow radius (in pixels)
    vec2  u_shadowOffset   = vec2(0.1, 0.1);// The pixel-space shadow offset from rectangle center

    // Colors
    vec4  u_colorBg     = vec4(0.0, 0.0, 0.0, 0.0);// The color of background
    vec4  u_colorRect   = instanceBuffer.objects[PushConstants.instanceIndex].bgColor;// The color of rectangle
    vec4  u_colorBorder = vec4(80.0 / 255.0, 96.0 / 255.0, 99.0 / 255.0, 1.0);// The color of (internal) border
    vec4  u_colorShadow = vec4(0.4, 0.4, 0.4, 1.0);// The color of shadow

    // =========================================================================

    vec2 halfSize = (u_rectSize / 2.0);// Rectangle extents (half of the size)

    // Calculate distance to edge.
    float distance = CircleSDF(u_rectCenter-halfSize, u_cornerRadius);

    // Smooth the result (free antialiasing).
    float smoothedAlpha = 1.0-smoothstep(0.0, u_edgeSoftness, distance);

    // Border 
    float borderAlpha   = 1.0-smoothstep(u_borderThickness - u_borderSoftness, u_borderThickness, abs(distance));

    // Apply a drop shadow effect.
    float shadowDistance  = CircleSDF(u_rectCenter-halfSize + u_shadowOffset, u_cornerRadius);
    float shadowAlpha      = 1.0-smoothstep(-u_shadowSoftness, u_shadowSoftness, shadowDistance);

    // -------------------------------------------------------------------------
    // Apply colors layer-by-layer: background <- shadow <- rect <- border.

    // Blend background with shadow
    vec4 res_shadow_color = mix(u_colorBg, vec4(u_colorShadow.rgb, shadowAlpha), shadowAlpha);

    // Blend (background+shadow) with rect
    //   Note:
    //     - Used 'min(u_colorRect.a, smoothedAlpha)' instead of 'smoothedAlpha'
    //       to enable rectangle color transparency
    vec4 res_shadow_with_rect_color = mix(
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
    vec4 res_shadow_with_rect_with_border = mix(
    res_shadow_with_rect_color,
    u_colorBorder,
    min(u_colorBorder.a, min(borderAlpha, smoothedAlpha))
    );

    // Finally output color
    outColor = res_shadow_with_rect_with_border;
}
