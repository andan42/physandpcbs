#version 430 core

//in
layout (location = 0) in vec3 avPos;
layout (location = 1) in vec2 avTexCoords;

//out
layout (location = 0) out vec2 vfTexCoords;

//uniform
layout (location = 0) uniform mat4 uvModel;

void main()
{
    gl_Position = uvModel * vec4(avPos, 1.0);
    vfTexCoords = avTexCoords;
}