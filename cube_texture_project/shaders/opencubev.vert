#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection*model*vec4(pos,1.0f);
    tex_coords = tex;
}