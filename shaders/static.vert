#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV0;
layout (location = 3) in vec2 inUV1;
layout (location = 4) in vec4 inJoint0;
layout (location = 5) in vec4 inWeight0;
layout (location = 6) in vec4 inColor0;

layout (set = 0, binding = 1) uniform UboInstance 
{
	mat4 mvp;
	vec4 color;

} uboInstance;

layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec2 outUV0;
layout (location = 3) out vec4 outColor0;
layout (location = 4) out vec4 outColor1;

void main() 
{	
	// Colors
	outColor0 = inColor0;
	outColor1 = uboInstance.color;

	// UV
	outUV0 = inUV0;

	// Normals
	outNormal = inNormal;

	gl_Position =  uboInstance.mvp * vec4(inPos, 1.0);
}
