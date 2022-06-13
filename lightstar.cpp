#ifndef SDL_GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#else
// Let SDL_opengl.h include glew functions
#define GL_GLEXT_PROTOTYPES 1
#endif

#ifdef SDL_GLSYMBOL
#include "sdl2_opengl.hh"
#else
#include <SDL_opengl.h>
#endif

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

#ifdef SDL_GLSYMBOL
#define D this->DrawFunctions->
#else
#define D
#endif

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

#ifdef SDL_GLSYMBOL
wurfel_lightstar::wurfel_lightstar (const struct DrawFunctions_t *DrawFunctions)
#else
wurfel_lightstar::wurfel_lightstar (void)
#endif
{
#ifdef SDL_GLSYMBOL
	this->DrawFunctions = DrawFunctions;
#endif

	D glGenVertexArrays (1, &vao);
	D glBindVertexArray (vao);

	D glGenBuffers (1, &vbo);
	D glBindBuffer (GL_ARRAY_BUFFER, vbo);
	D glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	D glGenBuffers (1, &ebo);
	D glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);
	D glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	GLuint vertexShader = D glCreateShader (GL_VERTEX_SHADER);
	D glShaderSource (vertexShader, 1, &lightstar_vertexSource, NULL);
	D glCompileShader (vertexShader);
	GLint status;
	D glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		D glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf ("Failed to compile lightstar_vertexSource:\n%s\n", buffer);
	}

	GLuint geometryShader = D glCreateShader (GL_GEOMETRY_SHADER);
	D glShaderSource (geometryShader, 1, &lightstar_geometrySource, NULL);
	D glCompileShader (geometryShader);
	D glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		D glGetShaderInfoLog(geometryShader, 512, NULL, buffer);
		printf ("Failed to compile lightstar_geometrySource:\n%s\n", buffer);
	}

	GLuint fragmentShader = D glCreateShader(GL_FRAGMENT_SHADER);
	D glShaderSource (fragmentShader, 1, &lightstar_fragmentSource, NULL);
	D glCompileShader (fragmentShader);
	D glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		D glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf ("Failed to compile lightstar_fragmentSource:\n%s\n", buffer);
	}

	shaderProgram = D glCreateProgram ();
	D glAttachShader (shaderProgram, vertexShader);
	D glAttachShader (shaderProgram, geometryShader);
	D glAttachShader (shaderProgram, fragmentShader);

	D glBindFragDataLocation (shaderProgram, 0, "outColor");
	D glLinkProgram (shaderProgram);
	D glDeleteShader (vertexShader); // The finished program is not affected by this
	D glDeleteShader (geometryShader);
	D glDeleteShader (fragmentShader);

	D glUseProgram (shaderProgram); /* program must be the activated once before glGetAttribLocation() and friends work */

	attrPosition = D glGetAttribLocation (shaderProgram, "position"); /* local position inside the model */
	uniModel = D glGetUniformLocation (shaderProgram, "model"); /* model location in the world */
	uniView  = D glGetUniformLocation (shaderProgram, "view");   /* camera angle + position */
	uniProj  = D glGetUniformLocation (shaderProgram, "proj");   /* project perspective */
	uniClock = D glGetUniformLocation (shaderProgram, "Clock");
	//uniMatrix  = glGetUniformLocation (lightstar_shaderProgram, "matrix");   /* project perspective */
}

void wurfel_lightstar::render (bool mirror, float const *proj, float const *view, float const *light1, float const *camera, float Clock)
{
	D glBindVertexArray (vao);
	D glBindBuffer (GL_ARRAY_BUFFER, vbo);
	D glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);

	D glUseProgram (shaderProgram);
	
	D glEnableVertexAttribArray (attrPosition);
	D glVertexAttribPointer (
		attrPosition,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof (vertices[0]),
		(const void *)offsetof (struct wurfel_lightstar::vertix_t, position)
	);

	D glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj);
	D glUniformMatrix4fv(uniView, 1, GL_FALSE, view);
	D glUniform1f (uniClock, Clock);

#if 0
	fprintf (stderr, "camera: %4f %4f %4f   light: %4f %4f %4f\n",
		camera[0], camera[1], camera[2],
		light1[0], light1[1], light1[2]);
#endif

#if 0
	D glm::mat4 model = glm::lookAt
	(
		D glm::vec3(light1[0], light1[1], light1[2]),
		D glm::vec3(camera[0], camera[1], camera[2]),
		D glm::vec3(0.0f, 0.0f, 1.0f)
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

	D glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	if (mirror == true)
	{
//		D glFrontFace (GL_CW);
	}

	D glEnable (GL_BLEND);

	D glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	D glDrawElements (GL_TRIANGLES, sizeof (elements)/(sizeof(GLuint)), GL_UNSIGNED_INT, 0);

	D glDisable (GL_BLEND);

	if (mirror == true)
	{
//		D glFrontFace (GL_CCW);
	}
}

wurfel_lightstar::~wurfel_lightstar (void)
{
	D glDeleteProgram (shaderProgram);
	D glDeleteBuffers(1, &ebo);
	D glDeleteBuffers(1, &vbo);
	D glDeleteVertexArrays(1, &vao);
}
