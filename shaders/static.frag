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
        
        if(inType == 1) {
                // Normalized pixel coordinates (from 0 to 1)
                vec2 uv = gl_FragCoord.xy/inResolution.xy;

                // Pixel color
                vec3 col = vec3(1,1,1);
                
                // Gradient across screen
                vec2 p = gl_FragCoord.xy;           // Point
                        vec2 c = inResolution.xy / 2.0;   // Center
                col *= (1.0 - length(c - p)/inResolution.x*0.7);
                        
                // 2-size grid
                col *= clamp(grid(gl_FragCoord.xy, 10., 0.5) *  grid(gl_FragCoord.xy, 50., 1.), grid_intensity, 1.0);
                
                // Output to screen
                outColor = vec4(col,1.0);
        } else if (inType == 2) {

                outColor = inColor1 * texture(Sampler2D, inUV0); 

        } else if (inType == 3) {

                outColor = inColor1;

        }
}