#version 430 core

//in
layout(location = 0) in vec2 vfTexCoords;

//out
layout(location = 0) out vec4 foColor;

//uniform
layout(location = 1) uniform sampler2D ufTexture;
layout(location = 2) uniform sampler2D ufMask;

void main()
{
    foColor = vec4(
        (texture(ufTexture, vfTexCoords).rrr + 1.0)/2.0,
        1.0);

    float val = texture(ufTexture, vfTexCoords).r;
    float mask = texture(ufMask, vfTexCoords).r;
    //turn val to heatmap 
    float R = max(val, 0.0);    
    float B = max(-val, 0.0);
    float G = 1.0 - abs(val);
    vec3 RGB = vec3(R, G, B);
    foColor = vec4( min(RGB * 2.0, 1.0), 1.0) * mask;

}