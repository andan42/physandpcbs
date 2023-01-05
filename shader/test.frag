#version 430 core

//in 
layout(location = 1) in vec4 vfNormal;
layout(location = 2) in vec4 vfColor;

//out
layout(location = 0) out vec4 foColor;

void main()
{
    foColor = vfColor;
    //depth buffer:
}