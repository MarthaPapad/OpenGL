#version 330 core

in vec2 tex_coords;
in float distance;

out vec4 FragColor;

uniform sampler2D tex;
uniform float density;

void main()
{
   vec4 fogColor = vec4(0.8, 0.8, 0.9, 0.1);
   vec4 texColor = texture(tex, tex_coords);

   float fogFactor = clamp(exp(-pow(distance * density,2.0)), 0.0, 1.0);
   FragColor = mix(fogColor, texColor, fogFactor);
}
