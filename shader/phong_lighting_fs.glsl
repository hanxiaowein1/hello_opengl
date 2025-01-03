#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;

// uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

in mat4 model_for_fragment;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    // vec4 light_pos_4 = model_for_fragment * vec4(viewPos, 0.0);
    // vec3 light_pos = vec3(light_pos_4.x, light_pos_4.y, light_pos_4.z);
    vec3 light_pos = viewPos;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos - FragPos);
    // float diff = max(dot(norm, lightDir), 0.0);
    float diff = abs(dot(norm, lightDir));
    vec3 diffuse = diff * lightColor;

    // specular
    // float specularStrength = 0.5;
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // vec3 specular = specularStrength * spec * lightColor;  
        
    // vec3 result = (ambient + diffuse + specular) * objectColor;
    vec3 result = (ambient + diffuse) * objectColor;
    // vec3 result = (ambient + diffuse + inner_diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
} 