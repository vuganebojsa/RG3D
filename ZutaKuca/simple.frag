#version 330 core

in vec4 channelCol;
out vec4 fragColor;  // Promenjeno ime iz outCol u fragColor


void main() {

    fragColor = channelCol;
}