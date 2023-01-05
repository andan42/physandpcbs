#version 430 core

//in
layout(location = 0) in vec4 avPos;
layout(location = 1) in vec4 avNormal;
layout(location = 2) in vec4 avColor;

//out
layout(location = 1) out vec4 vfNormal;
layout(location = 2) out vec4 vfColor;

//uniform
layout(location = 0) uniform float uvShine;
layout(location = 1) uniform mat4 uvModel;

void main()
{
    gl_Position = uvModel * avPos;
    vfNormal = avNormal;
    vfColor = avColor * uvShine;
}