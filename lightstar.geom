#version 150 core
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform mat4 view;
uniform mat4 proj;

// pass-through
in vec3 TexturePosition[];

out vec3 Position;
out vec3 _WorldPosition;

void main()
{
	for(int i = 0; i < gl_in.length(); i++)
	{
		gl_Position = proj * view * gl_in[i].gl_Position;
		Position = TexturePosition[i];
		_WorldPosition = gl_in[i].gl_Position.xyz;
		EmitVertex();
	}
	EndPrimitive();
}
