#version 450

layout(location=0)in vec3 inPosition;
layout(location=1)in vec3 inColor;

layout(location=0)out vec3 fragColor;
layout(location=1)out vec3 position;

void main(){
    gl_Position=vec4(inPosition,1.);
    fragColor=inColor;
    position=inPosition;
}