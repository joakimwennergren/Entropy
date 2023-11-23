#version 450

// Uniform Buffer Object
layout(binding=0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

// Vertex Attributes
layout(location=0)in vec3 inPosition;
layout(location=1)in vec3 inColor;
layout(location=2)in vec2 inTexCoord;

// Push Constant (per instance)
layout(push_constant) uniform constants
{
    mat4 modelMatrix;
    vec4 color;
    int textureId;
    
} pc;

// Attributes to fragment shader
layout(location=0)out vec3 fragColor;
layout(location=1)out vec2 fragTexCoord;
layout(location=2)out int texId;
layout(location=3)out vec4 color;

void main()
{
    // @todo refactor this..
    fragColor=inColor;
    fragTexCoord=inTexCoord;
    texId = pc.textureId;
    color = pc.color;

    gl_Position = ubo.proj * ubo.view * pc.modelMatrix * vec4(inPosition, 1.);
}