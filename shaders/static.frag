#version 450

//layout(set = 1, binding = 1) uniform sampler texSampler;
//layout(set = 1, binding = 2) uniform texture2D _texture;

layout(set = 1, binding = 2) uniform sampler2D Sampler2D;

layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV0;
layout (location = 3) in vec4 inColor0;
layout (location = 4) in vec4 inColor1;
layout (location = 5) flat in int inType;
layout (location = 6) in vec2 inResolution;
layout (location = 7) flat in uint inHasTexture;

layout(location = 0) out vec4 outColor;

float grid_intensity = 0.9;

// Thick lines 
float grid(vec2 fragCoord, float space, float gridWidth)
{
    vec2 p  = fragCoord - vec2(.5);
    vec2 size = vec2(gridWidth);

    vec2 a1 = mod(p - size, space);
    vec2 a2 = mod(p + size, space);
    vec2 a = a2 - a1;

    float g = min(a.x, a.y);
    return clamp(g, 0., 1.0);
}

float sdCircle(vec2 uv, float r, vec2 offset) {
    float x = uv.x - offset.x;
    float y = uv.y - offset.y;

    return length(vec2(x, y)) - r;
}

//---------------------------------------------------------
// draw rectangle frame with rounded edges
//---------------------------------------------------------
float roundedFrame (vec2 pos, vec2 uv, vec2 size, float radius, float thickness)
{
    float d = length(max(abs(uv - pos), size) - size) - radius;
    return smoothstep(0.55, 0.45, abs(d / thickness) * 5.0);
}

void main()
{
    // vec3 ambient = vec3(0.5, 0.5, 0.5);
    // vec3 lighting = vec3(0.0, 80.0, 0.0);

    // mat4 camera = inInvView;
    // vec3 cameraSource = vec3(camera[3].x, camera[3].y, camera[3].z);

    // // Diffuse
    // vec3 normal = normalize(inNormal);
    // vec3 lightColor = vec3(1.0, 1.0, 1.0);
    // vec3 lightSource = vec3(1.0, 1.0, 0.0);
    // float diffuseStrength = max(0.0, dot(lightSource, normal));
    // vec3 diffuse = diffuseStrength * lightColor;

    // // Specular
    // vec3 viewSource = normalize(cameraSource);
    // vec3 reflectSource = normalize(reflect(-lightSource, normal));
    // float specularStrength = max(0.0, dot(viewSource, reflectSource));
    // specularStrength = pow(specularStrength, 32.0);
    // vec3 specular = specularStrength * lightColor;


    // float distance = length(vec3(0.0, 5.0, 0.0) - inWorldPos.xyz);

    // float attenuation = 1.0 / (1.0 + 0.001 * distance +
    //             0.008 * (distance * distance));

    // ambient  *= attenuation;
    // diffuse  *= attenuation;
    // specular *= attenuation;

    // lighting = ambient * 2.0 + diffuse * 0.3 + specular * 0.3;

    // vec4 sampled = texture(sampler2D(_texture, texSampler), inUV0) * vec4(lighting, 1.0);

    // float value = sin(time * 0.008);
    // vec4 color = inColor0;
    // //vec4 tinting_pulsed = vec4(tint.r, tint.g, tint.b, value);
    // //color = inColor0 * tint;

    // outColor = color * sampled;


    // //vec4 sampled = texture(sampler2D(_texture, texSampler), inUV0);

    if (inType == 2) {

        vec4 frameColor = vec4(0.0, 0.0, 0.0, 1.0);
        float radius = 0.08;
        float thickness = 0.2;
        float s = 0.5 - (radius + (thickness / 10));
        vec2 size = vec2(s, s);
        vec2 pos = vec2(0.5, 0.5);

        // Normalize the pixel coordinates (this is "passTexCoords" in your case)
        vec2 uv = inUV0;//inUV0.xy/inResolution.xy;

        // (Note: iResolution.xy holds the x and y dimensions of the window in pixels)
        vec2 aspectRatio = vec2(inResolution.x/inResolution.y, 1.0);

        // In order to make sure visual distances are preserved, we multiply everything by aspectRatio
        //uv *= aspectRatio;
        //pos *= aspectRatio;
        //size *= aspectRatio;

        //--- rounded frame ---
        float intensity = roundedFrame(pos, uv, size, radius, thickness);
        vec4 col = mix(vec4(0.0, 0.0, 1.0, 0.0), frameColor, intensity);

        outColor = col;

        //outColor = inColor1 * texture(Sampler2D, inUV0);

    } else if (inType == 3) {
        outColor = inColor1;
    }
}
