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
	vec2 screen;
} uboView;

layout (binding = 1) uniform UboInstance 
{
	vec4 color;
	vec4 colorBorder;
	vec4 colorShadow;
	mat4 proj;
	mat4 view;
	mat4 invView;
	mat4 model;
	vec2 position;
	vec2 size;
	vec4 borderRadius;
	int shapeId;
	int time;
} uboInstance;

layout (set = 2, binding = 0) uniform UBONode {
	mat4 matrix;
	mat4 jointMatrix[MAX_NUM_JOINTS];
	float jointCount;
} node;

layout (location = 0) out vec3 outWorldPos;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec2 outUV0;
layout (location = 3) out vec2 outUV1;
layout (location = 4) out vec4 outColor0;
layout (location = 5) out mat4 outInvView;
layout (location = 12) out int outTime;
layout (location = 13) out vec4 outTint;

void main() 
{
	outColor0 = inColor0;
	outTint = uboInstance.color;
	outTime = uboInstance.time;
	
	vec4 locPos;

	if (node.jointCount > 0.0) {
		// Mesh is skinned
		mat4 skinMat = 
			inWeight0.x * node.jointMatrix[int(inJoint0.x)] +
			inWeight0.y * node.jointMatrix[int(inJoint0.y)] +
			inWeight0.z * node.jointMatrix[int(inJoint0.z)] +
			inWeight0.w * node.jointMatrix[int(inJoint0.w)];

		locPos = uboInstance.model * node.matrix * skinMat * vec4(inPos, 1.0);
		outNormal = normalize(transpose(inverse(mat3(uboInstance.model * node.matrix * skinMat))) * inNormal);
	} else {
		locPos = uboInstance.model * node.matrix * vec4(inPos, 1.0);
		outNormal = normalize(transpose(inverse(mat3(uboInstance.model * node.matrix))) * inNormal);
	}
	locPos.y = -locPos.y;
	outWorldPos = locPos.xyz / locPos.w;
	outUV0 = inUV0;
	outUV1 = inUV1;
	outInvView = uboInstance.invView;
	gl_Position =  uboInstance.proj * uboInstance.view * vec4(outWorldPos, 1.0);
}
