#version 330 core

in vec4 channelCol;
out vec4 fragColor;  // Promenjeno ime iz outCol u fragColor

uniform bool enableTransparency = false; 

void main() {
    vec4 color = channelCol;
    color.r = 0.2;
    color.g = 0.4;
    color.b = 0.8;

    if (enableTransparency) {
        color.a = 0.5;
    }
    else{
        color.a = 1.0;
    }
    fragColor = color;
}