#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec2 inUV1;
layout (location = 4) in vec4 inJoint0;
layout (location = 5) in vec4 inWeight0;
layout (location = 6) in vec4 inColor;

layout (location = 1) out vec3 Normal;
layout (location = 2) out vec2 UV;
layout (location = 3) out vec4 Color;
//push constants block
layout(push_constant) uniform constants {
    uint instanceIndex;
} PushConstants;
struct InstanceData{
    mat4 model;
    vec4 bgColor;
    vec4 borderColor;
    vec2 dimension;
    int type;
};

//all object matrices
layout(std140, set = 0, binding = 0) readonly buffer InstanceBuffer{
    InstanceData objects[];
} instanceBuffer;

layout (set = 0, binding = 1) uniform UboInstance
{
    mat4 perspective;
    mat4 view;

} uboInstance;

void main()
{
    // Feed fragment shader with vertex data
    Normal = inNormal;
    UV = inUV;
    Color = inColor;

    // Model matrix
    mat4 modelMatrix = instanceBuffer.objects[PushConstants.instanceIndex].model;

    // Calculate MVP matrix
    gl_Position = uboInstance.perspective * uboInstance.view * modelMatrix * vec4(inPos, 1.0);
}
