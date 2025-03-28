#version 330 core

in vec2 tex_coords;
out vec4 frag_col;
// texture sampler
uniform sampler2D tex;

void main()
{
	frag_col = texture(tex, tex_coords);
}