#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  

uniform sampler2D Texture;
uniform samplerCube skybox;

uniform vec3 cameraPos;

void main()
{    
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    // mirror
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);

    FragColor = texture(Texture, TexCoords);
}