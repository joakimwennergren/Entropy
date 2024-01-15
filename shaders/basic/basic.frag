#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in int texId;
layout(location = 3) in vec4 color;
layout(location = 4) in vec2 screen;
layout(location = 5) in vec3 position;

layout(set = 1, binding = 1) uniform sampler texSampler;
layout(set = 1, binding = 2) uniform texture2D _texture;

layout(location = 0) out vec4 outColor;

//---------------------------------------------------------
// draw rounded rectangle
//---------------------------------------------------------
float roundedBoxSDF(vec2 CenterPosition, vec2 Size, vec4 Radius)
{
    Radius.xy = (CenterPosition.x > 0.0) ? Radius.xy : Radius.zw;
    Radius.x  = (CenterPosition.y > 0.0) ? Radius.x  : Radius.y;
    
    vec2 q = abs(CenterPosition)-Size+Radius.x;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - Radius.x;
}

void main() 
{
    // For quads
    if (texId == -1) {
        // =========================================================================
        // Inputs (uniforms)

        vec2 iResolution = vec2(screen.x, screen.y);

        vec2  u_rectSize   = vec2(60.0, screen.y * 2.0);     // The pixel-space scale of the rectangle.
        vec2  u_rectCenter = vec2(position.x, position.y); // The pixel-space rectangle center location
        
        float u_edgeSoftness   = 2.0; // How soft the edges should be (in pixels). Higher values could be used to simulate a drop shadow.
        vec4  u_cornerRadiuses = vec4(0.0, 40.0, 40.0, 40.0); // The radiuses of the corners(in pixels): [topRight, bottomRight, topLeft, bottomLeft]
        
        // Border
        float u_borderThickness = 0.0; // The border size (in pixels) 
        float u_borderSoftness  = 0.0; // How soft the (internal) border should be (in pixels)
        
        // Shadow
        float u_shadowSoftness = 0.0;            // The (half) shadow radius (in pixels)
        vec2  u_shadowOffset   = vec2(0.0, 0.0); // The pixel-space shadow offset from rectangle center
        
        // Colors
        vec4  u_colorBg     = vec4(0.93, 0.93, 0.93, 0.0); // The color of background
        vec4  u_colorRect   = vec4(1.0,  0.30, 0.45, 0.6); // The color of rectangle
        vec4  u_colorBorder = vec4(0.7,  0.25, 0.55, 1.0); // The color of (internal) border
        vec4  u_colorShadow = vec4(0.4,  0.4,  0.4,  1.0); // The color of shadow
        
        // =========================================================================

        vec2 halfSize = (u_rectSize / 2.0); // Rectangle extents (half of the size)
        
        vec4 radius = u_cornerRadiuses; // Animated corners radiuses
        
            // -------------------------------------------------------------------------
    
        // Calculate distance to edge.   
        float distance = roundedBoxSDF(gl_FragCoord.xy - u_rectCenter, halfSize, radius);
        
        // Smooth the result (free antialiasing).
        float smoothedAlpha = 1.0-smoothstep(0.0, u_edgeSoftness, distance);
        
        // -------------------------------------------------------------------------
        // Border.
        
        float borderAlpha   = 1.0-smoothstep(u_borderThickness - u_borderSoftness, u_borderThickness, abs(distance));
        
        // -------------------------------------------------------------------------
        // Apply a drop shadow effect.
        
        float shadowDistance  = roundedBoxSDF(gl_FragCoord.xy - u_rectCenter + u_shadowOffset, halfSize, radius);
        float shadowAlpha 	  = 1.0-smoothstep(-u_shadowSoftness, u_shadowSoftness, shadowDistance);
        

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
    
    // For sprites
    if (texId == 1) {
        vec4 sampled = texture(sampler2D(_texture, texSampler), fragTexCoord);
        outColor = color * sampled;
    } 
    
    // For text
    if (texId == 2) {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(sampler2D(_texture, texSampler), fragTexCoord).r);
        outColor = color * sampled;
    }
}