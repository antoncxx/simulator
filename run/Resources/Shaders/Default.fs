#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  

uniform sampler2D Texture;

void main()
{    
    vec3 lightPos = vec3(0.f,70.f, 0.f);
    vec3 viewPos = vec3(1.f, 1.f, 1.f);
    vec3 lightColor = vec3(1.f, 1.f, 1.f);
    vec4 objectColor = texture(Texture, TexCoords);


    // ambient
    float ambientStrength = 0.9;
    vec3 ambient = ambientStrength * lightColor;
    
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    vec3 lll =  (ambient + diffuse + specular);
    FragColor = vec4(lll, 1.f) * objectColor;
}