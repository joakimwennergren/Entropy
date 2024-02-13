#version 450

#define MAX_NUM_JOINTS 128

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV0;
layout (location = 3) in vec2 inUV1;
layout (location = 4) in vec4 inJoint0;
layout (location = 5) in vec4 inWeight0;
layout (location = 6) in vec4 inColor0;

layout (binding = 0) uniform UboView 
{
	mat4 projection;
	mat4 view;
	mat4 invView;
} uboView;

layout (binding = 1) uniform UboInstance 
{
	vec4 color;
	mat4 model; 
} uboInstance;

layout (location = 0) out vec3 outWorldPos;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec2 outUV0;
layout (location = 3) out vec2 outUV1;
layout (location = 4) out vec4 outColor0;
layout (location = 5) out mat4 outInvView;

void main() 
{
	outColor0 = uboInstance.color;
	outUV0 = inUV0;
	gl_Position =  uboView.projection * uboView.view * uboInstance.model * vec4(inPos, 1.0);
}
