#version 150 core
in vec3 position;

out vec3 TexturePosition;

uniform mat4 model;

void main()
{
	TexturePosition = position.xyz;
	gl_Position = model * vec4(position, 1.0);
}
