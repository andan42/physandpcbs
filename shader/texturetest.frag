#version 430 core

//in
layout(location = 0) in vec2 vfTexCoords;

//out
layout(location = 0) out vec4 foColor;

//uniform
uniform sampler2D ufTexture;

void main()
{
    foColor = vec4(texture(ufTexture, vfTexCoords).rgr, 1.0);
}