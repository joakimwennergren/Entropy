#version 450

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

layout (location = 7) out vec3 nearPoint;
layout (location = 8) out vec3 farPoint;
layout (location = 9) out mat4 fragView;
layout (location = 14) out mat4 fragProj;

vec3 gridPlane[6] = vec3[] (
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() 
{
	fragView = uboInstance.view;
	fragProj = uboInstance.proj;
    vec3 p = gridPlane[gl_VertexIndex].xyz;
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, uboInstance.view, uboInstance.proj).xyz; // unprojecting on the near plane
    farPoint = UnprojectPoint(p.x, p.y, 1.0, uboInstance.view, uboInstance.proj).xyz; // unprojecting on the far plane
    gl_Position = vec4(p, 1.0); // using directly the clipped coordinates
}
