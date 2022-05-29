#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "lightstar.hh"

#include "lightstar-vert.cpp" /* vertix shader */
#include "lightstar-geom.cpp" /* geomery shader */
#include "lightstar-frag.cpp" /* fragment shader */

const struct wurfel_lightstar::vertix_t wurfel_lightstar::vertices[4] =
{
	/*    0 */ {{-1.000000, -1.000000, 0.000000}},
	/*    1 */ {{ 1.000000, -1.000000, 0.000000}},
	/*    2 */ {{ 1.000000,  1.000000, 0.000000}},
	/*    3 */ {{-1.000000,  1.000000, 0.000000}},
};

const struct wurfel_lightstar::element_t wurfel_lightstar::elements[2] =
{
	/*    0 */ {   1,    2,    0},
	/*    1 */ {   2,    3,    0},
};

wurfel_lightstar::wurfel_lightstar (void)
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
	glShaderSource (vertexShader, 1, &lightstar_vertexSource, NULL);
	glCompileShader (vertexShader);
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf ("Failed to compile lightstar_vertexSource:\n%s\n", buffer);
	}

	GLuint geometryShader = glCreateShader (GL_GEOMETRY_SHADER);
	glShaderSource (geometryShader, 1, &lightstar_geometrySource, NULL);
	glCompileShader (geometryShader);
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(geometryShader, 512, NULL, buffer);
		printf ("Failed to compile lightstar_geometrySource:\n%s\n", buffer);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource (fragmentShader, 1, &lightstar_fragmentSource, NULL);
	glCompileShader (fragmentShader);
	glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf ("Failed to compile lightstar_fragmentSource:\n%s\n", buffer);
	}

	shaderProgram = glCreateProgram ();
	glAttachShader (shaderProgram, vertexShader);
	glAttachShader (shaderProgram, geometryShader);
	glAttachShader (shaderProgram, fragmentShader);

	glBindFragDataLocation (shaderProgram, 0, "outColor");
	glLinkProgram (shaderProgram);
	glDeleteShader (vertexShader); // The finished program is not affected by this
	glDeleteShader (geometryShader);
	glDeleteShader (fragmentShader);

	glUseProgram (shaderProgram); /* program must be the activated once before glGetAttribLocation() and friends work */

	attrPosition = glGetAttribLocation (shaderProgram, "position"); /* local position inside the model */
	uniModel = glGetUniformLocation (shaderProgram, "model"); /* model location in the world */
	uniView  = glGetUniformLocation (shaderProgram, "view");   /* camera angle + position */
	uniProj  = glGetUniformLocation (shaderProgram, "proj");   /* project perspective */
	uniClock = glGetUniformLocation (shaderProgram, "Clock");
	//uniMatrix  = glGetUniformLocation (lightstar_shaderProgram, "matrix");   /* project perspective */
}

void wurfel_lightstar::render (bool mirror, float const *proj, float const *view, float const *light1, float const *camera, float Clock)
{
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);

	glUseProgram (shaderProgram);
	
	glEnableVertexAttribArray (attrPosition);
	glVertexAttribPointer (
		attrPosition,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof (vertices[0]),
		(const void *)offsetof (struct wurfel_lightstar::vertix_t, position)
	);

	glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, view);
	glUniform1f (uniClock, Clock);

#if 0
	fprintf (stderr, "camera: %4f %4f %4f   light: %4f %4f %4f\n",
		camera[0], camera[1], camera[2],
		light1[0], light1[1], light1[2]);
#endif

#if 0
	glm::mat4 model = glm::lookAt
	(
		glm::vec3(light1[0], light1[1], light1[2]),
		glm::vec3(camera[0], camera[1], camera[2]),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
#else
	glm::mat4 model = glm::mat4(1.0f);

	if (mirror == true)
	{
	    model = glm::scale(
		model,
		glm::vec3(1.0, 1.0, -1.0)
	    );
	}

	model = glm::translate (model, glm::vec3(light1[0], light1[1], light1[2]));
	model[0][0] = view[ 0];
	model[1][0] = view[ 1];
	model[2][0] = view[ 2];
	model[0][1] = view[ 4];
	model[1][1] = view[ 5];
	model[2][1] = view[ 6];
	model[0][2] = view[ 8];
	model[1][2] = view[ 9];
	model[2][2] = view[10];
	model = glm::scale (model, glm::vec3(0.1f, 0.1f, 0.1f));
#endif

#if 0
	fprintf (stderr,
		"%4f %4f %4f %4f\n"
		"%4f %4f %4f %4f\n"
		"%4f %4f %4f %4f\n"
		"%4f %4f %4f %4f\n",
		model[0][0], model[0][1], model[0][2], model[0][3],
		model[1][0], model[1][1], model[1][2], model[1][3],
		model[2][0], model[2][1], model[2][2], model[2][3],
		model[3][0], model[3][1], model[3][2], model[3][3]);
#endif

	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	if (mirror == true)
	{
//		glFrontFace (GL_CW);
	}

	glEnable (GL_BLEND);

	glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDrawElements (GL_TRIANGLES, sizeof (elements)/(sizeof(GLuint)), GL_UNSIGNED_INT, 0);

	glDisable (GL_BLEND);

	if (mirror == true)
	{
//		glFrontFace (GL_CCW);
	}
}

wurfel_lightstar::~wurfel_lightstar (void)
{
	glDeleteProgram (shaderProgram);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}
