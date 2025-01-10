layout(set = 1, binding = 2) uniform sampler2D Sampler2D;

layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV0;
layout (location = 3) in vec4 inColor0;
layout (location = 4) in vec4 inColor1;
layout (location = 5) flat in int objectType;
layout (location = 6) in vec2 inResolution;
layout (location = 7) flat in uint inHasTexture;

layout(location = 0) out vec4 outColor;

//layout(set = 1, binding = 1) uniform sampler texSampler;
//layout(set = 1, binding = 2) uniform texture2D _texture;s