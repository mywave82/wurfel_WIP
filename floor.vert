#version 150 core
in vec3 position;

out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	Position = position;
	gl_Position = proj * view * model * vec4(position, 0.2);
}
