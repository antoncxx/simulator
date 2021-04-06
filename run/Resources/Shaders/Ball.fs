#version 330 core

out vec4 FragColor;

in vec3 Pos;  

const vec4 COLOR1 = vec4(vec3(1.0, 0.0, 0.0), 1.0);
const vec4 COLOR2 = vec4(vec3(0.0, 1.0, 0.0), 1.0);


void main()
{  
    if (Pos.x > 0.) {
        if (Pos.y > 0.) {
            if (Pos.z > 0) {
                FragColor = COLOR2;
            } else {
                FragColor = COLOR1;
            }
        } else {
            if (Pos.z > 0) {
                FragColor = COLOR1;

            } else {
                FragColor = COLOR2;
            }
        }
    } else {
        if (Pos.y > 0.) {
            if (Pos.z > 0) {
                FragColor = COLOR1;
            } else {
                FragColor = COLOR2;
            }
        } else {
            if (Pos.z > 0) {
                FragColor = COLOR2;
            } else {
                FragColor = COLOR1;
            }
        }
    }
}


  	

    
 
        
