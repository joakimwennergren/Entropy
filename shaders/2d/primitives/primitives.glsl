void RoundedRectangleFrame()
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

    vec2  u_rectSize   = vec2(300.0, 300.0);// The pixel-space scale of the rectangle.
    vec2  u_rectCenter = (u_rectSize * inUV0);// The pixel-space rectangle center location
    vec2  u_offset = vec2(20, 20);

    float u_edgeSoftness   = 1.0;// How soft the edges should be (in pixels). Higher values could be used to simulate a drop shadow.
    vec4  u_cornerRadiuses = vec4(40.0, 40.0, 40.0, 40.0);// The radiuses of the corners(in pixels): [topRight, bottomRight, topLeft, bottomLeft]

    // Border
    float u_borderThickness = 5.0;// The border size (in pixels)
    float u_borderSoftness  = 2.0;// How soft the (internal) border should be (in pixels)

    // Shadow
    float u_shadowSoftness = 15.0;// The (half) shadow radius (in pixels)
    vec2  u_shadowOffset   = vec2(5.0, 5.0);// The pixel-space shadow offset from rectangle center

    // Colors
    vec4  u_colorBg     = vec4(0.0, 0.0, 0.0, 0.0);// The color of background
    vec4  u_colorRect   = vec4(1.0, 1.0, 1.0, 1.0);// The color of rectangle
    vec4  u_colorBorder = vec4(0.0, 0.0, 0.0, 1.0);// The color of (internal) border
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
