#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    vec3 intensity;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 3

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform bool displayWhite;
uniform bool displaySmoke=false;
uniform bool light1Status=true;
uniform bool light2Status=true;
uniform bool isDogDraw=false;
uniform bool isWindowDraw=false;
uniform bool isWindowTransparent=false;
uniform vec4 uSunColor;
uniform bool isDoorDraw = false;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
bool isPointInsideBox(vec3 point, vec3 minBounds, vec3 maxBounds);


void main()
{   
    vec4 texColora = texture(material.diffuse, TexCoords);
    if(texColora.a < 0.1 && !isDoorDraw)
        discard;
    if(displayWhite){
        FragColor = uSunColor;
    }else if(displaySmoke){
        FragColor = vec4(0.8, 0.8, 0.8, 0.4);
    }else if(isWindowDraw && isWindowTransparent){
        FragColor = vec4(0.8, 0.8, 0.8, 0.4);
            
    }
    else{
     // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
     vec3 houseMinBounds = vec3(-3.9f, 0.0f, -8.4f);
    vec3 houseMaxBounds = vec3(8.0f, 15.0f, -3.52f); 
    bool isInsideHouse = isPointInsideBox(FragPos, houseMinBounds, houseMaxBounds);
    // point lights
    for(int i = 0; i < 3; i++){
        if(i==0 && light1Status == false){
            continue;

        }
        else if(i==1 && light2Status == false){
                    continue;
        }
        if(light1Status == true){
            if(i==0 && FragPos.y > pointLights[0].position.y && distance(FragPos.y, pointLights[0].position.y) > 0.5f){
                continue;
            }
        }
        if(light2Status == true){
            if(i==1 && FragPos.y > pointLights[1].position.y && distance(FragPos.y, pointLights[1].position.y) > 0.5f){
                continue;
            }else if(i == 1 && FragPos.y < pointLights[1].position.y && distance(FragPos.y, pointLights[1].position.y) > 3.4f){
            continue;
            }
        }
       
        if(isInsideHouse == true && i == 2){
            continue;
        }
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 
        
    }
        if(FragPos.y < spotLight.position.y - 1.07f && !isInsideHouse){
            result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    

        }

        
        FragColor = vec4(result, 1.0);
    }
   
}
bool isPointInsideBox(vec3 point, vec3 minBounds, vec3 maxBounds) {
    return all(greaterThanEqual(point, minBounds)) && all(lessThanEqual(point, maxBounds));
}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * light.color;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * light.color;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)) * light.color;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * light.color * light.intensity;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * light.color * light.intensity;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)) * light.color * light.intensity;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * light.color;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * light.color;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords)) * light.color;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}