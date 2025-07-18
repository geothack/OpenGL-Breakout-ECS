#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
uniform float time;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    float strength = 0.01;
    gl_Position.x += cos(time * 10) * strength;        
    gl_Position.y += cos(time * 15) * strength;    
    TexCoords = aTexCoords;
}  