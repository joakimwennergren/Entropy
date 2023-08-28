#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

//push constants block
layout(push_constant)uniform constants
{
    mat4 modelMatrix;
    int textureId;
    
}PushConstants;

layout(binding = 1) uniform sampler texSampler;
layout(binding = 2) uniform texture2D textures[8];

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(sampler2D(textures[PushConstants.textureId], texSampler), fragTexCoord);
}