#version 450

layout(binding=0)uniform UniformBufferObject{
    mat4 model;
    mat4 view;
    mat4 proj;
}ubo;

layout(location=0)in vec3 inPosition;
layout(location=1)in vec3 inColor;
layout(location=2)in vec2 inTexCoord;

layout(push_constant)uniform constants
{
    mat4 modelMatrix;
    vec4 color;
    int textureId;
    
}PushConstants;

layout(location=0)out vec3 fragColor;
layout(location=1)out vec2 fragTexCoord;
layout(location=2)out int texId;
layout(location=3)out vec4 color;

void main(){
    gl_Position=ubo.proj*ubo.view*PushConstants.modelMatrix*vec4(inPosition,1.);
    fragColor=inColor;
    fragTexCoord=inTexCoord;
    texId = PushConstants.textureId;
    color = PushConstants.color;
}