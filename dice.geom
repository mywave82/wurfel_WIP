#version 150 core
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform mat4 view;
uniform mat4 proj;

// pass-through
in vec3 TexturePosition[];
in vec3 _vertexNormal[];

out vec3 Position;
out vec3 _WorldPosition;
out vec3 Normal;

void main()
{
	// if we wanted to calculate normal per face
	// vec3 d1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	// vec3 d2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	// vec3 facenormal = normalize(cross(d1,d2));

	for(int i = 0; i < gl_in.length(); i++)
	{
		gl_Position = proj * view * gl_in[i].gl_Position;
		_WorldPosition =            gl_in[i].gl_Position.xyz;
		Position = TexturePosition[i];
		Normal = _vertexNormal[i];
		EmitVertex();
	}
	EndPrimitive();
}
