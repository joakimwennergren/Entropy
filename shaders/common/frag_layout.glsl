layout(set = 1, binding = 2) uniform sampler2D Sampler2D;

layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;
layout (location = 3) in vec4 Color;

layout(location = 0) out vec4 outColor;

//layout(set = 1, binding = 1) uniform sampler texSampler;
//layout(set = 1, binding = 2) uniform texture2D _texture;s