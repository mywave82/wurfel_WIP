#version 150 core
in vec3 position;
in vec3 vertexNormal;

out vec3 TexturePosition;
out vec3 _vertexNormal;

uniform mat4 matrix;

uniform mat4 model;

void main()
{
	TexturePosition = (matrix * vec4(position.xyz, 1.0)).xyz;
	gl_Position = model * vec4(position, 1.0);
	_vertexNormal = normalize(mat3(model) * vertexNormal);
}
