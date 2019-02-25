#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int objectColor;

void main()
{         
	vec3 color;

	if(objectColor == 1){
		color = vec3(0.43f, 1.17f, 0.64f);
	}
	else{
		color = vec3(1.0f, 0.7f, 0.7f);
	}
    
    // ambient
    vec3 ambient = 0.2 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    
  
    vec3 specular = vec3(0.3) * spec; // assuming bright white light color


    FragColor = vec4(ambient + diffuse + specular, 1.0);
}