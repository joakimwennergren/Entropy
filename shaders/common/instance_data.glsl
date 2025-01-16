struct InstanceData{
    mat4 model;
    vec4 bgColor;
    vec4 borderColor;
    vec4 cornerRadiuses;
    vec2 dimension;
    int type;
};

//all object matrices
layout(std140, set = 0, binding = 0) readonly buffer InstanceBuffer{
    InstanceData objects[];
} instanceBuffer;
