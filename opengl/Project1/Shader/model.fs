#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec3 Color;


void main() {    
    FragColor = vec4(Color, 0);
}