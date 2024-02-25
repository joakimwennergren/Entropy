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

void main() 
{

        vec3 ambient = vec3(0.5, 0.5, 0.5);
        vec3 lighting = vec3(0.0, 0.0, 0.0);

        mat4 camera = inInvView;
        vec3 cameraSource = vec3(camera[3].x, camera[3].y, camera[3].z);

        // Diffuse
        vec3 normal = normalize(inNormal);
        vec3 lightColor = vec3(1.0, 1.0, 1.0);
        vec3 lightSource = vec3(1.0, 1.0, 0.0);
        float diffuseStrength = max(0.0, dot(lightSource, normal));
        vec3 diffuse = diffuseStrength * lightColor;

        // Specular
        vec3 viewSource = normalize(cameraSource);
        vec3 reflectSource = normalize(reflect(-lightSource, normal));
        float specularStrength = max(0.0, dot(viewSource, reflectSource));
        specularStrength = pow(specularStrength, 32.0);
        vec3 specular = specularStrength * lightColor;


        float distance = length(vec3(0.0, 5.0, 0.0) - inWorldPos.xyz);

        float attenuation = 1.0 / (1.0 + 0.001 * distance + 
                    0.008 * (distance * distance));  

        ambient  *= attenuation; 
        //diffuse  *= attenuation;
        specular *= attenuation;  

        lighting = ambient * 0.8 + diffuse * 1.0 + specular * 0.8;

        vec4 sampled = texture(sampler2D(_texture, texSampler), inUV0) * vec4(lighting, 1.0);
        outColor = inColor0 * sampled;
}