#ifndef SDL_GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#else
// Let SDL_opengl.h include glew functions
#define GL_GLEXT_PROTOTYPES 1
#endif

#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "dice.hh"

#include "dice-vert.cpp" /* vertix shader */
#include "dice-geom.cpp" /* geomery shader */
#include "dice-frag.cpp" /* fragment shader */

#include "dice-data.cpp" /* vertices w/normals and triangles */

wurfel_dice::wurfel_dice (void)
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
	glShaderSource (vertexShader, 1, &dice_vertexSource, NULL);
	glCompileShader (vertexShader);
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf ("Failed to compile dice_vertexSource:\n%s\n", buffer);
	}

	GLuint geometryShader = glCreateShader (GL_GEOMETRY_SHADER);
	glShaderSource (geometryShader, 1, &dice_geometrySource, NULL);
	glCompileShader (geometryShader);
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(geometryShader, 512, NULL, buffer);
		printf ("Failed to compile dice_geometrySource:\n%s\n", buffer);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource (fragmentShader, 1, &dice_fragmentSource, NULL);
	glCompileShader (fragmentShader);
	glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf ("Failed to compile dice_fragmentSource:\n%s\n", buffer);
	}

	shaderProgram = glCreateProgram ();
	glAttachShader (shaderProgram, vertexShader);
	glAttachShader (shaderProgram, geometryShader);
	glAttachShader (shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram (shaderProgram);
	glDeleteShader (vertexShader); // The finished program is not affected by this
	glDeleteShader (geometryShader);
	glDeleteShader (fragmentShader);

	glUseProgram (shaderProgram); /* program must be the activated once before glGetAttribLocation() and friends work */

	attrib_position = glGetAttribLocation (shaderProgram, "position"); /* local position inside the model */
	attrib_vertexNormal    = glGetAttribLocation (shaderProgram,    "vertexNormal");
	uniModel = glGetUniformLocation (shaderProgram, "model"); /* model location in the world */
	uniView  = glGetUniformLocation (shaderProgram, "view");   /* camera angle + position */
	uniProj  = glGetUniformLocation (shaderProgram, "proj");   /* project perspective */
	uniMatrix  = glGetUniformLocation (shaderProgram, "matrix");   /* project perspective */

	uniform_ambient    = glGetUniformLocation (shaderProgram, "ambient");
	uniform_lightPos   = glGetUniformLocation (shaderProgram, "lightPos");
	uniform_lightColor = glGetUniformLocation (shaderProgram, "lightColor");
}

void wurfel_dice::render (bool mirror, float const *proj, float const *view, float const *light1, float spin/*, float scale, float trax, float tray, float traz*/)
{
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);

	glUseProgram (shaderProgram);
	
	glEnableVertexAttribArray (attrib_position);
	glVertexAttribPointer (
		attrib_position,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof (vertices[0]),
		(const void *)offsetof (struct wurfel_dice::vertix_t, position)
	);

	glEnableVertexAttribArray (attrib_vertexNormal);
	glVertexAttribPointer (
		attrib_vertexNormal,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof (vertices[0]),
		(const void *)offsetof (struct wurfel_dice::vertix_t, normal)
	);

	/* we can move the texture around if we want to */
	glm::mat4 matrix = glm::mat4(1.0f);
	//matrix = glm::scale (matrix, glm::vec3(scale, scale, scale));
	//matrix = glm::translate (matrix, glm::vec3(trax, tray, traz));
	glUniformMatrix4fv(uniMatrix, 1, GL_FALSE, glm::value_ptr(matrix));

	glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, view);

	glm::mat4 model = glm::mat4(1.0f);

	if (mirror == true)
	{
	    model = glm::scale(
		model,
		glm::vec3(1.0, 1.0, -1.0)
	    );
	}

	// lift the dice up from the floor (diagonal of a dice is sqrt(3 * sideLength))
	model = glm::translate ( model, glm::vec3(0.0f, 0.0f, sqrt(3.0f)/2));
	// spin with time
	model = glm::rotate    ( model, (float)(spin * 2.0f * M_PI), glm::vec3(0.0f, 0.0f, 1.0f));
	// rotate the dice, to be at its tip

	model = glm::rotate    ( model, atanf(sqrtf(2.0f)) /*glm::radians(54.74f)*/, glm::vec3(0.5, 0.5, 0.0)); //glm::vec3(sqrtf(2.0f), sqrtf(2.0f), 0.0f));

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

	{
		float ambient[3] = {0.2f, 0.3f, 0.3f};
		//float lightPos[3] = {5.0f, 5.0f, 5.0f};
		float lightColor[3] = {0.6f, 0.9f, 0.9f};

		glUniform3fv (uniform_ambient,    1, ambient);
		glUniform3fv (uniform_lightPos,   1, /*lightPos*/ light1);
		glUniform3fv (uniform_lightColor, 1, lightColor);
	}		

	if (mirror == true)
	{
		glFrontFace (GL_CW);
	}
	glDrawElements (GL_TRIANGLES, sizeof (elements)/(sizeof(GLuint)), GL_UNSIGNED_INT, 0);
	if (mirror == true)
	{
		glFrontFace (GL_CCW);
	}
}

wurfel_dice::~wurfel_dice (void)
{
	glDeleteProgram (shaderProgram);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}
