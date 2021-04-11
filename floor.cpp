#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "floor.hh"

const struct wurfel_floor::vertix_t wurfel_floor::vertices[4] =
{
	{{-0.5200f,  0.5200f,  0.0000f}},
	{{ 0.5200f,  0.5200f,  0.0000f}},
	{{ 0.5200f, -0.5200f,  0.0000f}},
	{{-0.5200f, -0.5200f,  0.0000f}},
};

const struct wurfel_floor::element_t wurfel_floor::elements[2] =
{
	{2, 1, 0},
	{0, 3, 2}
};

const char *floor_vertexSource = R"glsl(
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
	})glsl";

const char *floor_fragmentSource = R"glsl(
	#version 150 core
	in vec3 Position;

	out vec4 outColor;
	uniform vec3 Color1;
	uniform vec3 Color2;
	uniform vec3 AvgColor;
	uniform float Frequency;

	uniform int Mode;

	void main()
	{
		if (Mode == 0)
		{
			outColor = vec4(0.0, 0.0, 0.0, 1.0);
			return;
		}

		vec3 color;
		vec2 fw = fwidth (Position.xy);
		vec2 fuzz = fw * Frequency * 2.0f;
		float fuzzMax = max(fuzz.s, fuzz.t);
		vec2 checkPos = fract(Position.xy * Frequency);
		//if (fuzzMax < 0.5)
		{
			vec2 p =          smoothstep(vec2(0.5), fuzz + vec2(0.5), checkPos) + 
			           (1.0 - smoothstep(vec2(0.0), fuzz,             checkPos));

			color = mix(Color1, Color2, p.x * p.y + (1.0 - p.x) * (1.0 - p.y));

			color = mix(color, AvgColor, smoothstep (0.125, 0.5, fuzzMax));

			outColor = vec4 (color, 0.2);
		//} else {
		//	outColor = vec4(AvgColor, 1.0);
		}
	})glsl";

wurfel_floor::wurfel_floor (void)
{
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);

	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers (1, &ebo);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vertexShader, 1, &floor_vertexSource, NULL);
	glCompileShader (vertexShader);
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf ("Failed to compile floor_vertexSource:\n%s\n", buffer);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource (fragmentShader, 1, &floor_fragmentSource, NULL);
	glCompileShader (fragmentShader);
	glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf ("Failed to compile floor_fragmentSource:\n%s\n", buffer);
	}

	shaderProgram = glCreateProgram ();
	glAttachShader (shaderProgram, vertexShader);
	glAttachShader (shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram (shaderProgram);
	glDeleteShader (vertexShader); // The finished program is not affected by this
	glDeleteShader (fragmentShader);

	glUseProgram (shaderProgram); /* program must be the activated once before glGetAttribLocation() and friends work */

	attrPosition = glGetAttribLocation (shaderProgram, "position"); /* local position inside the model */
	uniModel = glGetUniformLocation (shaderProgram, "model"); /* model location in the world */
	uniView  = glGetUniformLocation (shaderProgram, "view");   /* camera angle + position */
	uniProj  = glGetUniformLocation (shaderProgram, "proj");   /* project perspective */
	uniMode = glGetUniformLocation (shaderProgram, "Mode");
}

void wurfel_floor::render (bool mirror, float const *proj, float const *view)
{
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);

	glUseProgram (shaderProgram);

	glUniform3f (glGetUniformLocation (shaderProgram, "Color1"),     0.000f, 0.000f, 0.800f);
	glUniform3f (glGetUniformLocation (shaderProgram, "Color2"),     0.600f, 0.600f, 0.600f);
	glUniform3f (glGetUniformLocation (shaderProgram, "AvgColor"),   0.400f, 0.400f, 0.500f);
	glUniform1f (glGetUniformLocation (shaderProgram, "Frequency"),  8.0f);

	glEnableVertexAttribArray (attrPosition);
	glVertexAttribPointer (
		attrPosition,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof (vertices[0]),
		(const void *)offsetof (struct wurfel_floor::vertix_t, position)
	);

	glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, view);
	glUniform1i (uniMode, mirror == false ? 1 : 0);

	glm::mat4 model = glm::mat4(1.0f);
#if 0
	model = glm::rotate(
	    model,
	    glm::degrees(90.0f),
	    glm::vec3(1.0f, 0.0f, 0.0f)
	);
#endif
//	model = glm::rotate (model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
//	model = glm::scale (model, glm::vec3(10.0f));
//	model = glm::translate (model, glm::vec3(0.0f, 0.0f, sinf(glm::radians(angle))));
#if 0
	printf ("model[%7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f]\n",
		model[0][0], model[1][0], model[2][0], model[3][0],
		model[0][1], model[1][1], model[2][1], model[3][1],
		model[0][2], model[1][2], model[2][2], model[3][2],
		model[0][3], model[1][3], model[2][3], model[3][3]);
	printf ("view [%7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f]\n",
		view[0*4+0], view[1*4+0], view[2*4+0], view[3*4+0],
		view[0*4+1], view[1*4+1], view[2*4+1], view[3*4+1],
		view[0*4+2], view[1*4+2], view[2*4+2], view[3*4+2],
		view[0*4+3], view[1*4+3], view[2*4+3], view[3*4+3]);
	printf ("proj [%7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f\n"
	        "      %7.2f %7.2f %7.2f %7.2f]\n",
		proj[0*4+0], proj[1*4+0], proj[2*4+0], proj[3*4+0],
		proj[0*4+1], proj[1*4+1], proj[2*4+1], proj[3*4+1],
		proj[0*4+2], proj[1*4+2], proj[2*4+2], proj[3*4+2],
		proj[0*4+3], proj[1*4+3], proj[2*4+3], proj[3*4+3]);
#endif

	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	if (mirror == false)
	{
		glEnable (GL_BLEND);

		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	}
	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	if (mirror == false)
	{
		glDisable (GL_BLEND);
	}
}

wurfel_floor::~wurfel_floor (void)
{
	glDeleteProgram (shaderProgram);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}
