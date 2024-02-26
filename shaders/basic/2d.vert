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
} uboInstance;

layout (location = 0) out vec3 outWorldPos;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec2 outUV0;
layout (location = 3) out vec2 outUV1;
layout (location = 4) out vec4 outColor0;
layout (location = 5) out mat4 outInvView;
layout (location = 10) out vec4 outColorBorder;
layout (location = 11) out vec4 outColorShadow;
layout (location = 12) out vec2 outPosition;
layout (location = 13) out vec2 outSize;
layout (location = 14) out vec4 outBorderRadius;
layout (location = 15) out vec2 outScreen;
layout (location = 16) flat out int outShapeId;

void main() 
{
	outColor0 = uboInstance.color;
	outUV0 = inUV0;
	outColorBorder = uboInstance.colorBorder;
	outColorShadow = uboInstance.colorShadow;
	outPosition = uboInstance.position;
	outSize = uboInstance.size;
	outScreen = uboView.screen;
	outShapeId = uboInstance.shapeId;

	gl_Position =  uboInstance.proj * uboInstance.view * uboInstance.model * vec4(inPos, 1.0);
}
