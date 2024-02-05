#version 450

layout (location = 0) in vec3 inPos;
layout (location = 4) in vec4 joint0;
layout (location = 5) in vec4 weight0;
layout (location = 6) in vec3 inColor;

layout(std430, set = 2, binding = 0) readonly buffer JointMatrices {
	mat4 jointMatrices[];
};

layout(push_constant) uniform PushConsts {
	mat4 model;
} primitive;

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

void main() 
{
	outColor = inColor;
    modelColor = uboInstance.color;
	mat4 modelView = uboView.view * uboInstance.model;
	vec3 worldPos = vec3(modelView * vec4(inPos, 1.0));

	mat4 skinMat = 
		weight0.x * jointMatrices[int(joint0.x)] +
		weight0.y * jointMatrices[int(joint0.y)] +
		weight0.z * jointMatrices[int(joint0.z)] +
		weight0.w * jointMatrices[int(joint0.w)];

	gl_Position = uboView.projection * uboView.view * primitive.model * skinMat * vec4(inPos.xyz, 1.0);
}