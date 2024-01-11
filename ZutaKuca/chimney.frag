#version 330 core
out vec4 FragColor;

in vec3 chNormal;  
in vec3 chFragPos;  
in vec2 chUV;
in vec4 channelCol;

uniform vec3 uLightPos; 
uniform vec3 uViewPos; 
uniform vec3 uLightColor;

uniform sampler2D uDiffMap1;

void main()
{    

	FragColor = vec4(0.6, 0.4, 0.2, 1.0);
}

