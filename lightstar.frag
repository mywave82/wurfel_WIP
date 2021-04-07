#version 150 core
in vec3 Position;
in vec3 _WorldPosition;
out vec4 outColor;

void main()
{
	outColor = vec4(Position.x, Position.y, Position.z, 1.0);
}
