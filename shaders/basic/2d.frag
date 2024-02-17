#version 450


layout(set = 1, binding = 1) uniform sampler texSampler;
layout(set = 1, binding = 2) uniform texture2D _texture;

layout (location = 0) in vec3 inWorldPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV0;
layout (location = 3) in vec2 inUV1;
layout (location = 4) in vec4 inColor0;
layout (location = 5) in mat4 inInvView;


layout (location = 10) in vec4 outColorBorder;
layout (location = 11) in vec4 outColorShadow;
layout (location = 12) in vec2 outPosition;
layout (location = 13) in vec2 outSize;
layout (location = 14) in vec4 outBorderRadius;
layout (location = 15) in vec2 outScreen;
layout (location = 16) flat in int outShapeId;

layout(location = 0) out vec4 outColor;

// from https://iquilezles.org/articles/distfunctions
float roundedBoxSDF(vec2 CenterPosition, vec2 Size, float Radius) {
    return length(max(abs(CenterPosition)-Size+Radius,0.0))-Radius;
}

void main() 
{
    if(outShapeId == 0)
    {       
        // The pixel space scale of the rectangle.
        vec2 size = outSize * 2.0f;
        
        // the pixel space location of the rectangle.
        vec2 location = vec2(0.0, outScreen.y - size.y);

        // How soft the edges should be (in pixels). Higher values could be used to simulate a drop shadow.
        float edgeSoftness  = 1.0f;
        
        // The radius of the corners (in pixels).
        float radius =  0.0f;
        
        // Calculate distance to edge.   
        float distance 		= roundedBoxSDF(gl_FragCoord.xy - location - (size/2.0f), size / 2.0f, radius);
        
        // Smooth the result (free antialiasing).
        float smoothedAlpha =  1.0f-smoothstep(0.0f, edgeSoftness * 2.0f,distance);
        
        // Return the resultant shape.
        vec4 quadColor		= mix(vec4(1.0f, 0.0f, 0.0f, 0.0f), vec4(0.0f, 0.2f, 1.0f, smoothedAlpha), smoothedAlpha);
        
        // Apply a drop shadow effect.
        float shadowSoftness = 30.0f;
        vec2 shadowOffset 	 = vec2(0.0f, 10.0f);
        float shadowDistance = roundedBoxSDF(gl_FragCoord.xy - location + shadowOffset - (size/2.0f), size / 2.0f, radius);
        float shadowAlpha 	 = 1.0f-smoothstep(-shadowSoftness, shadowSoftness, shadowDistance);
        vec4 shadowColor 	 = vec4(0.4f, 0.4f, 0.4f, 1.0f);
        outColor			 = inColor0;//mix(quadColor, inColor0, smoothedAlpha);
    }

    if(outShapeId == 1)
    {
        vec4 sampled = texture(sampler2D(_texture, texSampler), inUV0);
        outColor = inColor0 * sampled;
    }

}