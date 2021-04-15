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

const struct wurfel_floor::vertix_t wurfel_floor::vertices1[4] =
{
	{{-0.5210f,  0.5210f,  0.0000f}},
	{{ 0.5210f,  0.5210f,  0.0000f}},
	{{ 0.5210f, -0.5210f,  0.0000f}},
	{{-0.5210f, -0.5210f,  0.0000f}},
};

const struct wurfel_floor::element_t wurfel_floor::elements1[2] =
{
	{2, 1, 0},
	{0, 3, 2}
};



const struct wurfel_floor::vertix_t wurfel_floor::vertices2[40] =
{
	{{-0.5690f,  0.5690f, -0.0050f}}, //  0
	{{-0.5700f,  0.5700f, -0.0050f}}, //  1
	{{-0.5700f,  0.5700f, -0.0040f}}, //  2
	{{-0.5700f,  0.5700f,  0.0240f}}, //  3
	{{-0.5700f,  0.5700f,  0.0250f}}, //  4
	{{-0.5690f,  0.5690f,  0.0250f}}, //  5
	{{-0.5210f,  0.5210f,  0.0250f}}, //  6
	{{-0.5200f,  0.5200f,  0.0250f}}, //  7
	{{-0.5200f,  0.5200f,  0.0240f}}, //  8
	{{-0.5200f,  0.5200f, -0.0010f}}, //  9

	{{ 0.5690f,  0.5690f, -0.0050f}}, // 10
	{{ 0.5700f,  0.5700f, -0.0050f}}, // 11
	{{ 0.5700f,  0.5700f, -0.0040f}}, // 12
	{{ 0.5700f,  0.5700f,  0.0240f}}, // 13
	{{ 0.5700f,  0.5700f,  0.0250f}}, // 14
	{{ 0.5690f,  0.5690f,  0.0250f}}, // 15
	{{ 0.5210f,  0.5210f,  0.0250f}}, // 16
	{{ 0.5200f,  0.5200f,  0.0250f}}, // 17
	{{ 0.5200f,  0.5200f,  0.0240f}}, // 18
	{{ 0.5200f,  0.5200f, -0.0010f}}, // 19

	{{ 0.5690f, -0.5690f, -0.0050f}}, // 20
	{{ 0.5700f, -0.5700f, -0.0050f}}, // 21
	{{ 0.5700f, -0.5700f, -0.0040f}}, // 22
	{{ 0.5700f, -0.5700f,  0.0240f}}, // 23
	{{ 0.5700f, -0.5700f,  0.0250f}}, // 24
	{{ 0.5690f, -0.5690f,  0.0250f}}, // 25
	{{ 0.5210f, -0.5210f,  0.0250f}}, // 26
	{{ 0.5200f, -0.5200f,  0.0250f}}, // 27
	{{ 0.5200f, -0.5200f,  0.0240f}}, // 28
	{{ 0.5200f, -0.5200f, -0.0010f}}, // 29

	{{-0.5690f, -0.5690f, -0.0050f}}, // 30
	{{-0.5700f, -0.5700f, -0.0050f}}, // 31
	{{-0.5700f, -0.5700f, -0.0040f}}, // 32
	{{-0.5700f, -0.5700f,  0.0240f}}, // 33
	{{-0.5700f, -0.5700f,  0.0250f}}, // 34
	{{-0.5690f, -0.5690f,  0.0250f}}, // 35
	{{-0.5210f, -0.5210f,  0.0250f}}, // 36
	{{-0.5200f, -0.5200f,  0.0250f}}, // 37
	{{-0.5200f, -0.5200f,  0.0240f}}, // 38
	{{-0.5200f, -0.5200f, -0.0010f}}, // 39
};

const struct wurfel_floor::element_t wurfel_floor::elements2[74] =
{
	/* bottom center */
	{ 0, 10, 30},
	{10, 20, 30},

	/* bottom thin edge */
	{ 0, 11, 10},
	{ 0,  1, 11},

	{10, 21, 20,},
	{20, 11, 21,},

	{20, 31, 30},
	{20, 21, 31},

	{30,  1, 0},
	{30, 31, 1},

	/* bottom side edge */
	{ 1, 12, 11},
	{ 1,  2, 12},

	{11, 22, 21},
	{11, 12, 22},

	{21, 32, 31},
	{21, 22, 32},

	{31,  2,  1},
	{31, 32,  2},

	/* middle side edge */
	{ 2, 13, 12},
	{ 2,  3, 13},

	{12, 23, 22},
	{12, 13, 23},

	{22, 33, 32},
	{22, 23, 33},

	{32,  3,  2},
	{32, 33,  3},

	/* top side edge */
	{ 3, 14, 13},
	{ 3,  4, 14},

	{13, 24, 23},
	{13, 14, 24},

	{23, 34, 33},
	{23, 24, 34},

	{33,  4,  3},
	{33, 34,  4},

	/* top outer edge */
	{ 4, 15, 14},
	{ 4,  5, 15},

	{14, 25, 24},
	{14, 15, 25},

	{24, 35, 34},
	{24, 25, 35},

	{34,  5,  4},
	{34, 35,  5},

	/* top central edge */
	{ 5, 16, 15},
	{ 5,  6, 16},

	{15, 26, 25},
	{15, 16, 26},

	{25, 36, 35},
	{25, 26, 36},

	{35,  6,  5},
	{35, 36,  6},

	/* top inner edge */
	{ 6, 17, 16},
	{ 6,  7, 17},

	{16, 27, 26},
	{16, 17, 27},

	{26, 37, 36},
	{26, 27, 37},

	{36,  7,  6},
	{36, 37,  7},

	/* inside top edge */
	{ 7, 18, 17},
	{ 7,  8, 18},

	{17, 28, 27},
	{17, 18, 28},

	{27, 38, 37},
	{27, 28, 38},

	{37,  8,  7},
	{37, 38,  8},

	/* inside edge */
	{ 8, 19, 18},
	{ 8,  9, 19},

	{18, 29, 28},
	{18, 19, 29},

	{28, 39, 38},
	{28, 29, 39},

	{38,  9,  8},
	{38,  39, 9}
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
	glGenVertexArrays (2, vao);
	glGenBuffers (2, vbo);
	glGenBuffers (2, ebo);

	glBindVertexArray (vao[0]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(elements1), elements1, GL_STATIC_DRAW);

	glBindVertexArray (vao[1]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(elements2), elements2, GL_STATIC_DRAW);

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
	glBindVertexArray (vao[0]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo[0]);

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
		sizeof (vertices1[0]),
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

	if (mirror == false)
	{
		glBindVertexArray (vao[1]);
		glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo[1]);

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
			sizeof (vertices1[0]),
			(const void *)offsetof (struct wurfel_floor::vertix_t, position)
		);

		glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, view);
		glUniform1i (uniMode, mirror == false ? 1 : 0);

		glm::mat4 model = glm::mat4(1.0f);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glDrawElements (GL_TRIANGLES, 74*3, GL_UNSIGNED_INT, 0);
	}
}

wurfel_floor::~wurfel_floor (void)
{
	glDeleteProgram (shaderProgram);
	glDeleteBuffers(2, ebo);
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(2, vao);
}
