#version 330 core
out vec4 FragColor;

in vec3 v_norm;
in vec3 v_pos;



uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

void main()
{           
    vec3 color =  vec3(1.0, 1.0, 1.0);
    // ambient
    vec3 ambient = 0.05 * color;
   
   // diffuse
    vec3 lightDir = normalize(lightPos * color);
    vec3 normal = normalize(v_norm);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
	// specular
    vec3 viewDir = normalize(viewPos - v_pos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    
	if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
   
   vec3 specular = vec3(0.3) * spec * color; 
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}