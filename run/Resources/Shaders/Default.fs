#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  

uniform sampler2D Texture;
//uniform samplerCube skybox;

uniform vec3 cameraPos;

void main()
{    
	//mirror : apply to ball mb?
    //vec3 I = normalize(FragPos - cameraPos);
    //vec3 R = reflect(I, normalize(Normal));
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);

    vec3 lightPos = vec3(100.f, 200.f, 100.f);
  	vec3 lightColor = vec3(1.f, 1.f, 1.f);
  	vec4 objectColor = texture(Texture, TexCoords);

  	// ambient
    float ambientStrength = 1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 1;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    FragColor = vec4(ambient + diffuse + specular, 1.f) * objectColor;
}


  	

    
 
        
