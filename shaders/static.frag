#version 450

//layout(set = 1, binding = 1) uniform sampler texSampler;
//layout(set = 1, binding = 2) uniform texture2D _texture;

layout(set = 1, binding = 2) uniform sampler2D Sampler2D;

layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV0;
layout (location = 3) in vec4 inColor0;
layout (location = 4) in vec4 inColor1;
layout (location = 5) flat in int inType;
layout (location = 6) in vec2 inResolution;
layout (location = 7) flat in uint inHasTexture;

layout(location = 0) out vec4 outColor;

float grid_intensity = 0.9;

// Thick lines 
float grid(vec2 fragCoord, float space, float gridWidth)
{
    vec2 p  = fragCoord - vec2(.5);
    vec2 size = vec2(gridWidth);

    vec2 a1 = mod(p - size, space);
    vec2 a2 = mod(p + size, space);
    vec2 a = a2 - a1;

    float g = min(a.x, a.y);
    return clamp(g, 0., 1.0);
}

// from https://iquilezles.org/articles/distfunctions
// additional thanks to iq for optimizing conditional block for individual corner radii!
float roundedBoxSDF(vec2 CenterPosition, vec2 Size, vec4 Radius)
{
    Radius.xy = (CenterPosition.x > 0.0) ? Radius.xy : Radius.zw;
    Radius.x  = (CenterPosition.y > 0.0) ? Radius.x  : Radius.y;
    
    vec2 q = abs(CenterPosition)-Size+Radius.x;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - Radius.x;
}

void main()
{
    // vec3 ambient = vec3(0.5, 0.5, 0.5);
    // vec3 lighting = vec3(0.0, 80.0, 0.0);

    // mat4 camera = inInvView;
    // vec3 cameraSource = vec3(camera[3].x, camera[3].y, camera[3].z);

    // // Diffuse
    // vec3 normal = normalize(inNormal);
    // vec3 lightColor = vec3(1.0, 1.0, 1.0);
    // vec3 lightSource = vec3(1.0, 1.0, 0.0);
    // float diffuseStrength = max(0.0, dot(lightSource, normal));
    // vec3 diffuse = diffuseStrength * lightColor;

    // // Specular
    // vec3 viewSource = normalize(cameraSource);
    // vec3 reflectSource = normalize(reflect(-lightSource, normal));
    // float specularStrength = max(0.0, dot(viewSource, reflectSource));
    // specularStrength = pow(specularStrength, 32.0);
    // vec3 specular = specularStrength * lightColor;


    // float distance = length(vec3(0.0, 5.0, 0.0) - inWorldPos.xyz);

    // float attenuation = 1.0 / (1.0 + 0.001 * distance +
    //             0.008 * (distance * distance));

    // ambient  *= attenuation;
    // diffuse  *= attenuation;
    // specular *= attenuation;

    // lighting = ambient * 2.0 + diffuse * 0.3 + specular * 0.3;

    // vec4 sampled = texture(sampler2D(_texture, texSampler), inUV0) * vec4(lighting, 1.0);

    // float value = sin(time * 0.008);
    // vec4 color = inColor0;
    // //vec4 tinting_pulsed = vec4(tint.r, tint.g, tint.b, value);
    // //color = inColor0 * tint;

    // outColor = color * sampled;


    // //vec4 sampled = texture(sampler2D(_texture, texSampler), inUV0);

    if (inType == 1) {

        // Normalized pixel coordinates (from 0 to 1)
        vec2 uv = gl_FragCoord.xy/inResolution.xy;

        // Pixel color
        vec3 col = vec3(1, 1, 1);

        // Gradient across screen
        vec2 p = gl_FragCoord.xy;// Point
        vec2 c = inResolution.xy / 2.0;// Center
        col *= (1.0 - length(c - p)/inResolution.x*0.7);

        // 2-size grid
        col *= clamp(grid(gl_FragCoord.xy, 10., 0.5) *  grid(gl_FragCoord.xy, 50., 1.), grid_intensity, 1.0);

        // Output to screen
        outColor = vec4(col, 1.0);

    } else if (inType == 2) {

        // https://www.shadertoy.com/view/fsdyzB

        // =========================================================================
        // Inputs (uniforms)

        vec2  u_rectSize   = vec2(250.0, 250.0);     // The pixel-space scale of the rectangle.
        vec2  u_rectCenter = (inResolution.xy / 2.0); // The pixel-space rectangle center location
        
        float u_edgeSoftness   = 2.0; // How soft the edges should be (in pixels). Higher values could be used to simulate a drop shadow.
        vec4  u_cornerRadiuses = vec4(40.0, 40.0, 40.0, 40.0); // The radiuses of the corners(in pixels): [topRight, bottomRight, topLeft, bottomLeft]
        
        // Border
        float u_borderThickness = 5.0; // The border size (in pixels) 
        float u_borderSoftness  = 2.0; // How soft the (internal) border should be (in pixels)
        
        // Shadow
        float u_shadowSoftness = 30.0;            // The (half) shadow radius (in pixels)
        vec2  u_shadowOffset   = vec2(0.0, 5.0); // The pixel-space shadow offset from rectangle center
        
        // Colors
        vec4  u_colorBg     = vec4(0.93, 0.93, 0.93, 1.0); // The color of background
        vec4  u_colorRect   = vec4(1.0,  0.30, 0.45, 1.0); // The color of rectangle
        vec4  u_colorBorder = vec4(0.7,  0.25, 0.55, 1.0); // The color of (internal) border
        vec4  u_colorShadow = vec4(0.4,  0.4,  0.4,  1.0); // The color of shadow
        
        // =========================================================================

        vec2 halfSize = (u_rectSize / 2.0); // Rectangle extents (half of the size)
        
        vec4 radius = u_cornerRadiuses; // Animated corners radiuses
        
        // -------------------------------------------------------------------------
        
        // Calculate distance to edge.   
        float distance = roundedBoxSDF(vec2(1.0, 1.0), vec2(1, 1), radius);
        
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
        
        /*
        vec4 frameColor = vec4(0.0, 0.0, 0.0, 1.0);
        float radius = 0.08;
        float thickness = 0.2;
        float s = 0.5 - (radius + (thickness / 10));
        vec2 size = vec2(s, s);
        vec2 pos = vec2(0.5, 0.5);

        // Normalize the pixel coordinates (this is "passTexCoords" in your case)
        vec2 uv = inUV0; //inUV0.xy/inResolution.xy;

        // (Note: iResolution.xy holds the x and y dimensions of the window in pixels)
        vec2 aspectRatio = vec2(inResolution.x/inResolution.y, 1.0);

        // In order to make sure visual distances are preserved, we multiply everything by aspectRatio
        uv *= aspectRatio;
        pos *= aspectRatio;
        size *= aspectRatio;

        //--- rounded frame ---
        float intensity = roundedFrame(pos, uv, size, radius, thickness);
        vec4 col = mix(vec4(0.0, 0.0, 0.0, 0.0), frameColor, intensity);

        outColor = col;

        //outColor = inColor1 * texture(Sampler2D, inUV0);

        */

    } else if (inType == 3) {

        outColor = inColor1;

    }
}