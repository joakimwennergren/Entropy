#version 460

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV0;
layout (location = 3) in vec2 inUV1;
layout (location = 4) in vec4 inJoint0;
layout (location = 5) in vec4 inWeight0;
layout (location = 6) in vec4 inColor0;

struct ObjectData{
	mat4 model;
	vec4 color;
};

//all object matrices
layout(std140,set = 0, binding = 0) readonly buffer ObjectBuffer{
	ObjectData objects[];
} objectBuffer;

layout (set = 0, binding = 1) uniform UboInstance 
{
	mat4 perspective;
	mat4 view;

} uboInstance;

layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec2 outUV0;
layout (location = 3) out vec4 outColor0;
layout (location = 4) out vec4 outColor1;

void main() 
{	
	// Colors
	outColor0 = inColor0;
	outColor1 = objectBuffer.objects[gl_BaseInstance].color;

	// UV
	outUV0 = inUV0;

	// Normals
	outNormal = inNormal;

	mat4 modelMatrix = objectBuffer.objects[gl_BaseInstance].model;

	gl_Position =  uboInstance.perspective * uboInstance.view * modelMatrix * vec4(inPos, 1.0);
}
