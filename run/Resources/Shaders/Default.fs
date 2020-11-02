#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
void main()
{
	vec3 lightPos = vec3(100.f,200.f, 100.f);
	vec3 viewPos = vec3(1.f, 1.f, 1.f);
	vec3 lightColor = vec3(1.f, 1.f, 1.f);
	vec3 objectColor = vec3(1.f, 0.f, 0.f);


    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 