#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in int texId;
layout(location = 3) in vec4 color;

layout(binding = 1) uniform sampler texSampler;
layout(binding = 2) uniform texture2D textures[8];

layout(location = 0) out vec4 outColor;

void main() {
    if(texId == -1)
    {
        outColor = color;
    } else 
    {
        outColor = texture(sampler2D(textures[texId], texSampler), fragTexCoord);
    }

}