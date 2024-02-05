#version 450


layout (location = 4) in vec4 outColor0;

layout (location = 0) out vec4 outFragColor;

void main() 
{
	outFragColor = outColor0;
}