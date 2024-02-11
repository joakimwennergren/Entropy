#version 450


layout(set = 1, binding = 1) uniform sampler texSampler;
layout(set = 1, binding = 2) uniform texture2D _texture;

layout (location = 0) in vec3 inWorldPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV0;
layout (location = 3) in vec2 inUV1;
layout (location = 4) in vec4 inColor0;
layout (location = 5) in mat4 inInvView;

layout(location = 0) out vec4 outColor;

void main() 
{
    vec4 sampled = texture(sampler2D(_texture, texSampler), inUV0);
    outColor = inColor0 * sampled;
}