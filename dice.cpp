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

#include "dice.hh"

#include "dice-vert.cpp" /* vertix shader */
#include "dice-geom.cpp" /* geomery shader */
#include "dice-frag.cpp" /* fragment shader */

#include "dice-data.cpp" /* vertices w/normals and triangles */

#ifdef SDL_GLSYMBOL
#define D this->DrawFunctions->
#else
#define D
#endif

#ifdef SDL_GLSYMBOL
wurfel_dice::wurfel_dice (const struct DrawFunctions_t *DrawFunctions)
#else
wurfel_dice::wurfel_dice ()
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
	D glShaderSource (vertexShader, 1, &dice_vertexSource, NULL);
	D glCompileShader (vertexShader);
	GLint status;
	D glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		D glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf ("Failed to compile dice_vertexSource:\n%s\n", buffer);
	}

	GLuint geometryShader = D glCreateShader (GL_GEOMETRY_SHADER);
	D glShaderSource (geometryShader, 1, &dice_geometrySource, NULL);
	D glCompileShader (geometryShader);
	D glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		D glGetShaderInfoLog(geometryShader, 512, NULL, buffer);
		printf ("Failed to compile dice_geometrySource:\n%s\n", buffer);
	}

	GLuint fragmentShader = D glCreateShader(GL_FRAGMENT_SHADER);
	D glShaderSource (fragmentShader, 1, &dice_fragmentSource, NULL);
	D glCompileShader (fragmentShader);
	D glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		char buffer[512];
		D glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf ("Failed to compile dice_fragmentSource:\n%s\n", buffer);
	}

	shaderProgram = D glCreateProgram ();
	D glAttachShader (shaderProgram, vertexShader);
	D glAttachShader (shaderProgram, geometryShader);
	D glAttachShader (shaderProgram, fragmentShader);

	D glBindFragDataLocation(shaderProgram, 0, "outColor");
	D glLinkProgram (shaderProgram);
	D glDeleteShader (vertexShader); // The finished program is not affected by this
	D glDeleteShader (geometryShader);
	D glDeleteShader (fragmentShader);

	D glUseProgram (shaderProgram); /* program must be the activated once before glGetAttribLocation() and friends work */

	attrib_position     = D glGetAttribLocation (shaderProgram, "position"); /* local position inside the model */
	attrib_vertexNormal = D glGetAttribLocation (shaderProgram,    "vertexNormal");
	uniModel  = D glGetUniformLocation (shaderProgram, "model"); /* model location in the world */
	uniView   = D glGetUniformLocation (shaderProgram, "view");   /* camera angle + position */
	uniProj   = D glGetUniformLocation (shaderProgram, "proj");   /* project perspective */
	uniMatrix = D glGetUniformLocation (shaderProgram, "matrix");   /* project perspective */

	uniform_ambient    = D glGetUniformLocation (shaderProgram, "ambient");
	uniform_lightPos   = D glGetUniformLocation (shaderProgram, "lightPos");
	uniform_lightColor = D glGetUniformLocation (shaderProgram, "lightColor");
	uniform_hilight1   = D glGetUniformLocation (shaderProgram, "hilight1");
	uniform_hilight2   = D glGetUniformLocation (shaderProgram, "hilight2");
	uniform_hilight3   = D glGetUniformLocation (shaderProgram, "hilight3");
}

void wurfel_dice::elapse (float time)
{
	timer[0][0] = fmodf (timer[0][0] + time * 0.4134f, (float)(2.0f * M_PI));
	timer[0][1] = fmodf (timer[0][1] + time * 0.0241f, (float)(2.0f * M_PI));
	timer[0][2] = fmodf (timer[0][2] + time * 0.0141f, (float)(2.0f * M_PI));
	timer[0][3] = fmodf (timer[0][3] + time * 0.0491f, (float)(2.0f * M_PI));

	timer[1][0] = fmodf (timer[1][0] + time * 0.0142f, (float)(2.0f * M_PI));
	timer[1][1] = fmodf (timer[1][1] + time * 0.0584f, (float)(2.0f * M_PI));
	timer[1][2] = fmodf (timer[1][2] + time * 0.2101f, (float)(2.0f * M_PI));
	timer[1][3] = fmodf (timer[1][3] + time * 0.0631f, (float)(2.0f * M_PI));

	timer[2][0] = fmodf (timer[2][0] + time * 0.0742f, (float)(2.0f * M_PI));
	timer[2][1] = fmodf (timer[2][1] + time * 0.1864f, (float)(2.0f * M_PI));
	timer[2][2] = fmodf (timer[2][2] + time * 0.0144f, (float)(2.0f * M_PI));
	timer[2][3] = fmodf (timer[2][3] + time * 0.0525f, (float)(2.0f * M_PI));
}

void wurfel_dice::render (bool mirror, float const *proj, float const *view, float const *light1, float spin/*, float scale, float trax, float tray, float traz*/)
{
	D glBindVertexArray (vao);
	D glBindBuffer (GL_ARRAY_BUFFER, vbo);
	D glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ebo);

	D glUseProgram (shaderProgram);
	
	D glEnableVertexAttribArray (attrib_position);
	D glVertexAttribPointer (
		attrib_position,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof (vertices[0]),
		(const void *)offsetof (struct wurfel_dice::vertix_t, position)
	);

	D glEnableVertexAttribArray (attrib_vertexNormal);
	D glVertexAttribPointer (
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
	D glUniformMatrix4fv(uniMatrix, 1, GL_FALSE, glm::value_ptr(matrix));

	D glUniformMatrix4fv(uniProj, 1, GL_FALSE, proj);
	D glUniformMatrix4fv(uniView, 1, GL_FALSE, view);

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

	D glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	{
		float ambient[3] = {0.2f, 0.3f, 0.3f};
		//float lightPos[3] = {5.0f, 5.0f, 5.0f};
		float lightColor[3] = {0.6f, 0.9f, 0.9f};

		float hilight1[4] = {2.34f  + 20.0f*sin(timer[0][0]),  3.45f + 15.0f*sin(timer[0][1]), -4.56f + 15.0f*sin(timer[0][2]),  0.00f + 10.0f*sin(timer[0][3])};
		float hilight2[4] = {6.54f  + 15.0f*sin(timer[1][0]), -5.43f + 20.0f*sin(timer[1][1]),  4.32f + 15.0f*sin(timer[1][2]), -4.49f + 10.0f*sin(timer[1][3])};
		float hilight3[4] = {-4.00f + 15.0f*sin(timer[2][0]),  4.00f + 15.0f*sin(timer[2][1]),  4.00f + 20.0f*sin(timer[2][2]),  5.89f + 10.0f*sin(timer[2][3])};

		D glUniform3fv (uniform_ambient,    1, ambient);
		D glUniform3fv (uniform_lightPos,   1, /*lightPos*/ light1);
		D glUniform3fv (uniform_lightColor, 1, lightColor);
		D glUniform4fv (uniform_hilight1,   1, hilight1);
		D glUniform4fv (uniform_hilight2,   1, hilight2);
		D glUniform4fv (uniform_hilight3,   1, hilight3);
	}		

	if (mirror == true)
	{
		D glFrontFace (GL_CW);
	}
	D glDrawElements (GL_TRIANGLES, sizeof (elements)/(sizeof(GLuint)), GL_UNSIGNED_INT, 0);
	if (mirror == true)
	{
		D glFrontFace (GL_CCW);
	}
}

wurfel_dice::~wurfel_dice (void)
{
	D glDeleteProgram (shaderProgram);
	D glDeleteBuffers(1, &ebo);
	D glDeleteBuffers(1, &vbo);
	D glDeleteVertexArrays(1, &vao);
}
