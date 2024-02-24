#version 450


layout(set = 1, binding = 1) uniform sampler texSampler;
layout(set = 1, binding = 2) uniform texture2D _texture;

layout (location = 0) in vec3 inWorldPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV0;
layout (location = 3) in vec2 inUV1;
layout (location = 4) in vec4 inColor0;
layout (location = 5) in mat4 inInvView;

layout(location = 0) out vec4 outColor;

bool inTile(vec2 p, float tileSize) {
    // draw tiles that are tileSize in size
    vec2 ptile = step(0.5, fract(0.5 * p / tileSize));
    return ptile.x == ptile.y;
}

void main() 
{ 
    vec2 p = gl_FragCoord.xy;
    // smooth out the sine motion with a sqrt; tile size of 16
    float tileSize = sqrt(sin(1.0 * 2.) / 2. + 1.) * 16.;
    // if diagonals match, draw as a black square
    if (inTile(gl_FragCoord.xy, tileSize)) outColor = vec4(0.);
    else outColor = vec4(1.);
}