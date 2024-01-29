#version 450

layout (location = 0) in vec3 inPos;
layout (location = 6) in vec3 inColor;

layout (binding = 0) uniform UboView 
{
	mat4 projection;
	mat4 view;
} uboView;

layout (binding = 1) uniform UboInstance 
{
	vec4 color;
	mat4 model; 
} uboInstance;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec4 modelColor;

out gl_PerVertex 
{
	vec4 gl_Position;   
};

void main() 
{
	outColor = inColor;
    modelColor = uboInstance.color;
	mat4 modelView = uboView.view * uboInstance.model;
	vec3 worldPos = vec3(modelView * vec4(inPos, 1.0));
	gl_Position = uboView.projection * modelView * vec4(inPos.xyz, 1.0);
}