#version 330 core
out vec4 FragColor;

in vec3 chNormal;  
in vec3 chFragPos;  
in vec2 chUV;
  
uniform vec3 uLightPos; 
uniform vec3 uViewPos; 
uniform vec3 uLightColor;

uniform sampler2D uDiffMap1;
uniform bool displayWhite = false;
void main()
{    

     if (displayWhite) {
        // Display white color if the condition is true
        FragColor = vec4(0.9, 0.9, 0.9, 1.0);
    } else {
        // Your existing lighting calculations
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * uLightColor;
        
        vec3 norm = normalize(chNormal);
        vec3 lightDir = normalize(uLightPos - chFragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * uLightColor;
        
        float specularStrength = 0.5;
        vec3 viewDir = normalize(uViewPos - chFragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * uLightColor;  

        FragColor = texture(uDiffMap1, chUV) * vec4(ambient + diffuse + specular, 1.0);
    }
}

