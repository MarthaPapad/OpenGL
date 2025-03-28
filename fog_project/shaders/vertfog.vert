#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 tex_coords;
out float distance;

uniform mat4 view;
uniform mat4 rotation;
uniform mat4 projection;
uniform mat4 translation;

void main()
{
    vec4 worldPos = translation * rotation * vec4(pos, 1.0f);
    vec4 posRelativetoCam = view * worldPos;
    gl_Position = projection * posRelativetoCam;
    tex_coords = tex;

    distance = length(posRelativetoCam.xyz);
}