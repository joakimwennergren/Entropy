#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in int texId;
layout(location = 3) in vec4 color;

layout(binding = 1) uniform sampler texSampler;
layout(binding = 2) uniform texture2D _texture;
//layout(set = 1, binding = 2) uniform texture2D _texture;

layout(location = 0) out vec4 outColor;

void main() 
{
    if(texId == -1) {
        outColor = color;
    }  else if(texId == 1) {
        vec4 sampled = texture(sampler2D(_texture, texSampler), fragTexCoord);
        outColor = color * sampled;
    } else if(texId == 2) {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(sampler2D(_texture, texSampler), fragTexCoord).r);
        outColor = color * sampled;
    }
}