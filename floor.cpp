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

#include "border-vert.cpp"
#include "border-frag.cpp"

#include "floor-vert.cpp"
#include "floor-frag.cpp"

wurfel_floor::wurfel_floor (void)
{
	GLint status;
	GLuint vertexShader;
	GLuint fragmentShader;

	glGenVertexArrays (2, vao);
	glGenBuffers (2, vbo);
	glGenBuffers (2, ebo);

	/* initialize the floor */

	glBindVertexArray (vao[0]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(elements1), elements1, GL_STATIC_DRAW);

	vertexShader = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vertexShader, 1, &floor_vertexSource, NULL);
	glCompileShader (vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf ("Failed to compile floor_vertexSource:\n%s\n", buffer);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource (fragmentShader, 1, &floor_fragmentSource, NULL);
	glCompileShader (fragmentShader);
	glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf ("Failed to compile floor_fragmentSource:\n%s\n", buffer);
	}

	shaderProgram[0] = glCreateProgram ();
	glAttachShader (shaderProgram[0], vertexShader);
	glAttachShader (shaderProgram[0], fragmentShader);

	glBindFragDataLocation(shaderProgram[0], 0, "outColor");
	glLinkProgram (shaderProgram[0]);
	glDeleteShader (vertexShader); // The finished program is not affected by this
	glDeleteShader (fragmentShader);

	glUseProgram (shaderProgram[0]); /* program must be the activated once before glGetAttribLocation() and friends work */

	attrPosition[0] = glGetAttribLocation (shaderProgram[0], "position"); /* local position inside the model */
	uniModel[0] = glGetUniformLocation (shaderProgram[0], "model"); /* model location in the world */
	uniView[0] = glGetUniformLocation (shaderProgram[0], "view");   /* camera angle + position */
	uniProj[0] = glGetUniformLocation (shaderProgram[0], "proj");   /* project perspective */
	uniMode = glGetUniformLocation (shaderProgram[0], "Mode");


	/* initialize the border */

	glBindVertexArray (vao[1]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);
	glBufferData (GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(elements2), elements2, GL_STATIC_DRAW);

	vertexShader = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vertexShader, 1, &border_vertexSource, NULL);
	glCompileShader (vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf ("Failed to compile border_vertexSource:\n%s\n", buffer);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource (fragmentShader, 1, &border_fragmentSource, NULL);
	glCompileShader (fragmentShader);
	glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf ("Failed to compile border_fragmentSource:\n%s\n", buffer);
	}

	shaderProgram[1] = glCreateProgram ();
	glAttachShader (shaderProgram[1], vertexShader);
	glAttachShader (shaderProgram[1], fragmentShader);

	glBindFragDataLocation(shaderProgram[1], 0, "outColor");
	glLinkProgram (shaderProgram[1]);
	glDeleteShader (vertexShader); // The finished program is not affected by this
	glDeleteShader (fragmentShader);

	glUseProgram (shaderProgram[1]); /* program must be the activated once before glGetAttribLocation() and friends work */

	attrPosition[1] = glGetAttribLocation (shaderProgram[1], "position"); /* local position inside the model */
	uniModel[1] = glGetUniformLocation (shaderProgram[1], "model"); /* model location in the world */
	uniView[1]  = glGetUniformLocation (shaderProgram[1], "view");   /* camera angle + position */
	uniProj[1]  = glGetUniformLocation (shaderProgram[1], "proj");   /* project perspective */
	uniMatrix = glGetUniformLocation (shaderProgram[1], "matrix");   /* texture-matrix */
}

void wurfel_floor::render (bool mirror, float const *proj, float const *view)
{
	glBindVertexArray (vao[0]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo[0]);

	glUseProgram (shaderProgram[0]);

	glUniform3f (glGetUniformLocation (shaderProgram[0], "Color1"),     0.000f, 0.000f, 0.800f);
	glUniform3f (glGetUniformLocation (shaderProgram[0], "Color2"),     0.600f, 0.600f, 0.600f);
	glUniform3f (glGetUniformLocation (shaderProgram[0], "AvgColor"),   0.400f, 0.400f, 0.500f);
	glUniform1f (glGetUniformLocation (shaderProgram[0], "Frequency"),  8.0f);

	glEnableVertexAttribArray (attrPosition[0]);
	glVertexAttribPointer (
		attrPosition[0],
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof (vertices1[0]),
		(const void *)offsetof (struct wurfel_floor::vertix_t, position)
	);

	glUniformMatrix4fv(uniProj[0], 1, GL_FALSE, proj);
	glUniformMatrix4fv(uniView[0], 1, GL_FALSE, view);
	glUniform1i (uniMode, mirror == false ? 1 : 0);

	glm::mat4 model = glm::mat4(1.0f);

	glUniformMatrix4fv(uniModel[0], 1, GL_FALSE, glm::value_ptr(model));

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

		glUseProgram (shaderProgram[1]);

		glEnableVertexAttribArray (attrPosition[1]);
		glVertexAttribPointer (
			attrPosition[1],
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof (vertices2[0]),
			(const void *)offsetof (struct wurfel_floor::vertix_t, position)
		);

		glUniformMatrix4fv(uniProj[1], 1, GL_FALSE, proj);
		glUniformMatrix4fv(uniView[1], 1, GL_FALSE, view);

		glm::mat4 model = glm::mat4(1.0f);
		glUniformMatrix4fv(uniModel[1], 1, GL_FALSE, glm::value_ptr(model));

		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate (matrix, glm::vec3(10.0f, 10.0f, 100.0f));
		matrix = glm::scale (matrix, glm::vec3(38.55f, 38.55f, 38.55f));
		glUniformMatrix4fv(uniMatrix, 1, GL_FALSE, glm::value_ptr(matrix));

		glDrawElements (GL_TRIANGLES, 74*3, GL_UNSIGNED_INT, 0);
	}
}

wurfel_floor::~wurfel_floor (void)
{
	glDeleteProgram (shaderProgram[0]);
	glDeleteProgram (shaderProgram[1]);
	glDeleteBuffers(2, ebo);
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(2, vao);
}
